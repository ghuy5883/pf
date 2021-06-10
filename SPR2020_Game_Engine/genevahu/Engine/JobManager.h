#pragma once
#include "Worker.h"
#include "Job.h"
#include <queue>
#include <atomic>

class JobManager
{
	public:
		JobManager();
		~JobManager();
		void Begin();
		void End();
		void AddJob(Job* pJob);
		Job* PopJob();
		void CompleteJob(Job* pJob);
		void WaitForJobs();

	private:
		#define WORKERCOUNT 4
		Worker* m_Workers[4];
		std::queue<Job*> m_Jobs;
		bool isActive;
		std::atomic_int m_JobCounter;
};

