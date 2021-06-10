#pragma once
#include "stdafx.h"


class Profiler
{
	public:

		// Timer class
		class Timer {
			public:
				void Start();
				void Stop();
				void Reset();
				const std::string& GetName() const;
				double GetTime_ms() const;
				double GetMax_ms() const;
				double GetAvg_ms() const;

			private:
				Timer();
				~Timer();
				friend class Profiler;
				std::string m_Name;
				double m_MostRecentFrameLength;
				double m_LongestFrameLength;
				double m_TotalTime;
				int m_TotalFramesCaptured;
				std::chrono::high_resolution_clock::time_point m_RecordTimeStart;
		};

		class ScopedTimer {

			public:
				ScopedTimer(Timer* timer);
				~ScopedTimer();

			private:
				Timer* m_Timer;
		};

		// Functions
		static Profiler* Get();
		Timer* GetTimer(const std::string& name);
		void ResetAll();


	private:
		Profiler();
		~Profiler();
		std::unordered_map<std::string, Timer*> m_Timers;
		FILE* jsonStream;
		static bool jsonFirstLine;
		static std::string jsonWriteString;
};

#define PROFILE_SCOPE(name) \
Profiler::ScopedTimer name##_scope(Profiler::Get()->GetTimer(std::string(#name)))


