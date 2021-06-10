#include "stdafx.h"
#include "JobManager.h"
#include <mutex>

std::mutex g_Mutex;

JobManager::JobManager()
{
	// Default inactive
	isActive = false;

	// Add workers to job manager
	for (int i = 0; i < WORKERCOUNT; i++) {
		Worker* addWorker = new Worker();
		m_Workers[i] = addWorker;
	}

	// Set static job manager of all workers to this manager
	m_Workers[0]->SetJobManager(this);

	// Start workers
	Begin();
}

JobManager::~JobManager()
{
	End();
}

void JobManager::Begin()
{
	// Begin each worker
	for (int i = 0; i < WORKERCOUNT; i++) {
		m_Workers[i]->Begin();
	}

	// Set as active
	isActive = true;
}

void JobManager::End()
{
	// Set as inactive
	isActive = false;
	Worker::ShutDownSignal();

	// End each worker
	for (int i = 0; i < WORKERCOUNT; i++) {
		m_Workers[i]->End();
	}

	// Delete every worker after they have properly ended
	int iter = 0;
	while (iter < WORKERCOUNT) {
		if (m_Workers[iter]->CheckShutDown()) {
			delete m_Workers[iter];
			iter++;
		}
	}
}

void JobManager::AddJob(Job* pJob)
{
	g_Mutex.lock();
	// Push a job to the end of the queue
	m_Jobs.emplace(pJob);
	g_Mutex.unlock();
	m_JobCounter++;
}

Job* JobManager::PopJob()
{
	g_Mutex.lock();
	Job* temp = nullptr;
	if (!m_Jobs.empty()) {
		// Pull off the front of the job queue
		temp = m_Jobs.front();
		// Remove from the queue
		m_Jobs.pop();
		m_JobCounter--;
	}
	g_Mutex.unlock();
	return temp;
}

void JobManager::CompleteJob(Job* pJob)
{
	delete pJob;
}

void JobManager::WaitForJobs()
{
	// Check if there are any jobs to do
	if (m_JobCounter == 0) {
		// Sleep while waiting for jobs
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}
