#pragma once
#include <thread>
#include "Job.h"

class JobManager;
class Worker
{
	public:
		Worker();
		~Worker();
		void Begin();
		void End();
		static void Loop();
		static void ShutDownSignal();
		static void SetJobManager(JobManager* manager);
		bool CheckShutDown();
		

	private:
		std::thread m_Worker;
		static bool isAwake;
		static JobManager* m_JobManager;
		bool m_ShutDown;
};

