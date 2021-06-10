#include "stdafx.h"
#include "Worker.h"
#include "JobManager.h"

bool Worker::isAwake = true;
JobManager* Worker::m_JobManager = nullptr;
Worker::Worker()
	: m_Worker()
{
	m_ShutDown = false;	
}

Worker::~Worker()
{
}

void Worker::Begin()
{
	// Create a thread
	m_Worker = std::thread(&Worker::Loop);
}

void Worker::End()
{
	// Join the thread and release any resources
	m_Worker.join();
	m_ShutDown = true;
}

void Worker::Loop()
{
	// Sleep while waiting for shutdown signal
	while (isAwake) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		// Pull the next job (if there is one) and call DoIt
		Job* temp = m_JobManager->PopJob();
		if (temp != nullptr) {
			temp->DoIt();
			// When job is done, mark as complete and set the job for deletion
			m_JobManager->CompleteJob(temp);
		}	
	}
	return;
}

void Worker::ShutDownSignal()
{
	isAwake = false;
}

void Worker::SetJobManager(JobManager* manager)
{
	m_JobManager = manager;
}

bool Worker::CheckShutDown()
{
	return m_ShutDown;
}
