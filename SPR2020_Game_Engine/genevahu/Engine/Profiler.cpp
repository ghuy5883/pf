#include "stdafx.h"
#include "Profiler.h"
#include "stdafx.h"

/* -----------------------------------------------------------------------
	FUNCTIONS FOR THE TIMER CLASS INSIDE THE PROFILER CLASS
-------------------------------------------------------------------------- */
void Profiler::Timer::Start()
{
	// Record the starting time.
	m_RecordTimeStart = std::chrono::high_resolution_clock::now();

	// Get the timestamp for the "B" event for JSON.
	long long beginTimeStamp = m_RecordTimeStart.time_since_epoch().count();

	// Convert from nanoseconds to microseconds.
	beginTimeStamp /= 1000;
	std::string beginTimeStampString = std::to_string(beginTimeStamp);
	std::string pIDDummy = std::to_string(0);

	// String to add to json.
	std::string jsonString = "{\"name\": \"" + m_Name +
		"\", \"cat\": \"PERF\", \"ph\": \"B\", \"ts\": " + beginTimeStampString +
		", \"pid\": " + pIDDummy + ", \"tid\": " + pIDDummy +
		"}";

	// Unless this is the first entry, pre-pend entry with comma and newline
	if (jsonFirstLine != true) {
		jsonString = ",\n" + jsonString;
	}
	else {
		jsonFirstLine = false;
	}
	jsonWriteString += jsonString;

}

void Profiler::Timer::Stop()
{
	// Record the stopping time.
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

	// Get the frame duration in nanoseconds.
	double frameDurationNS = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_RecordTimeStart).count();

	// Convert nanoseconds to milliseconds.
	frameDurationNS /= 1000000.0;

	// Modify the frame length
	m_MostRecentFrameLength += frameDurationNS;

	// Get the timestamp for the "E" event for JSON.
	long long endTimeStamp = end.time_since_epoch().count();

	// Convert from nanoseconds to microseconds.
	endTimeStamp /= 1000;
	std::string endTimeStampString = std::to_string(endTimeStamp);
	std::string pIDDummy = std::to_string(0);

	// String to add to json.
	std::string jsonString = "{\"name\": \"" + m_Name +
		"\", \"cat\": \"PERF\", \"ph\": \"E\", \"ts\": " + endTimeStampString +
		", \"pid\": " + pIDDummy + ", \"tid\": " + pIDDummy +
		"}";

	// Unless this is the first entry, pre-pend entry with comma and newline
	if (jsonFirstLine != true) {
		jsonString = ",\n" + jsonString;
	}
	else {
		jsonFirstLine = false;
	}
	jsonWriteString += jsonString;
}

void Profiler::Timer::Reset()
{
	// Add the total time for the current frame to the total frame time.
	m_TotalTime += m_MostRecentFrameLength;

	// Increase the count of the number of frames.
	m_TotalFramesCaptured++;

	// Update longest frame time.
	if (m_MostRecentFrameLength > m_LongestFrameLength) {
		m_LongestFrameLength = m_MostRecentFrameLength;
	}

	// Reset current frame's duration to 0.
	m_MostRecentFrameLength = 0.0;
}

const std::string& Profiler::Timer::GetName() const
{
	return m_Name;
}

double Profiler::Timer::GetTime_ms() const
{
	return m_MostRecentFrameLength;
}

double Profiler::Timer::GetMax_ms() const
{
	return m_LongestFrameLength;
}

double Profiler::Timer::GetAvg_ms() const
{
	return m_TotalTime / m_TotalFramesCaptured;
}

Profiler::Timer::Timer()
{
	m_Name = "";
	m_MostRecentFrameLength = 0.0;
	m_LongestFrameLength = 0.0;
	m_TotalTime = 0.0;
	m_TotalFramesCaptured = 0;
}

Profiler::Timer::~Timer()
{
	// Destructor.
}

/* -----------------------------------------------------------------------
	FUNCTIONS FOR THE PROFILER CLASS
-------------------------------------------------------------------------- */

Profiler* Profiler::Get()
{
	static Profiler m_Profile;
	// Return local static profiler.
	return &m_Profile;
}

Profiler::Timer* Profiler::GetTimer(const std::string& name)
{
	// Search for the requested timer.
	std::unordered_map<std::string, Timer*>::iterator iter = m_Timers.find(name);
	if (iter != m_Timers.end())
		// Timer located.
		return iter->second;
	else {
		// Timer not located, create a new one and return that.
		Profiler::Timer* m_newTimer = new Profiler::Timer();
		m_newTimer->m_Name = name;

		// Add the new timer to the map.
		m_Timers[name] = m_newTimer;

		// Return the newly created timer.
		return m_newTimer;
	}
}

void Profiler::ResetAll()
{
	// Loop through all the timers and call reset on them.
	std::unordered_map<std::string, Timer*>::iterator it = m_Timers.begin();

	while (it != m_Timers.end()) {
		it->second->Reset();
		it++;
	}

}

bool Profiler::jsonFirstLine = true;
std::string Profiler::jsonWriteString = "";
Profiler::Profiler()
{
	// Constructor.

	// Open and write to a json file for duration events.
	fopen_s(&jsonStream, "profile.json", "w");

	// Begin JSON format.
	jsonWriteString = "[";
}

Profiler::~Profiler()
{
	// Destructor.

	// Open and write to a text file of timer data.
	FILE* outStream;
	fopen_s(&outStream, "profile.txt", "w");

	// Loop through the timers
	std::unordered_map<std::string, Timer*>::iterator iter = m_Timers.begin();
	while (iter != m_Timers.end()) {
		// Get name of each timer
		std::string timerName = iter->second->GetName();

		// Get average and maximum time in ms.
		double averageTime = iter->second->GetAvg_ms();
		std::string averageTimeStr = std::to_string(averageTime);
		double maxTime = iter->second->GetMax_ms();
		std::string maxTimeStr = std::to_string(maxTime);

		// Convert to string.
		std::string dataString = "Name: " + timerName + ", Avg " + averageTimeStr + " (ms), max " + maxTimeStr + " (ms)\n";
		const char* dataStringChar = dataString.c_str();

		// Write data with printf.
		if (outStream != NULL) {
			fprintf(outStream, dataStringChar);
		}

		iter++;
	}

	// Close stream. 
	if (outStream != NULL) {
		fclose(outStream);
	}
	if (jsonStream != NULL) {
		// End JSON format.
		jsonWriteString += "]";
		fprintf(jsonStream, jsonWriteString.c_str());

		// Close stream. 
		fclose(jsonStream);
	}
}

/* -----------------------------------------------------------------------
	FUNCTIONS FOR THE SCOPED TIMER CLASS
-------------------------------------------------------------------------- */
Profiler::ScopedTimer::ScopedTimer(Timer* timer)
{
	m_Timer = timer;
	m_Timer->Start();
}

Profiler::ScopedTimer::~ScopedTimer()
{
	m_Timer->Stop();
}
