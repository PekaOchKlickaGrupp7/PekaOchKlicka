#include "stdafx.h"
#include "TimerManager.h"
#include <Windows.h>

namespace CU
{
	namespace TimeSys
	{
		TimerManager::TimerManager()
		{
			Init();
		}

		void TimerManager::Init()
		{
			QueryPerformanceFrequency(&myFrequency);
			myFrequency.QuadPart /= 1000000; // I want micro seconds
			myMasterTimer.SetFrequency(myFrequency.QuadPart);

			QueryPerformanceCounter(&myTick1);
			QueryPerformanceCounter(&myTick2);

			myElapsedTime = 0.0;
			Time newTime(myFrequency.QuadPart);
			newTime.SetData(((myTick1.QuadPart - myTick2.QuadPart)));
			myMasterTimer.Start(newTime);
		}


		TimerManager::~TimerManager()
		{
		}

		double TimerManager::GetElapsedTime()
		{
			return myElapsedTime;
		}

		const Timer& TimerManager::GetTimer(unsigned char anID) const
		{
			return myTimers[anID];
		}

		const Timer& TimerManager::GetMasterTimer() const
		{
			return myMasterTimer;
		}

		unsigned char TimerManager::CreateTimer()
		{
			Timer newTimer;
			newTimer.SetFrequency(myFrequency.QuadPart);
			newTimer.Start(myMasterTimer.GetTime());
			myTimers.push_back(newTimer);
			return static_cast<unsigned char>(myTimers.size() - 1); // Do not make more than 255 timers or God's gonna put you down.
			// Seriously though don't do it or it will wrap around.
		}

		void TimerManager::StartTimers(unsigned char ID)
		{
			myTimers[ID].Start(myMasterTimer.GetTime());
		}

		void TimerManager::StopTimers(unsigned char ID)
		{
			myTimers[ID].Stop();
		}

		const char TimerManager::GetCountOfTimers() const
		{
			return static_cast<unsigned char>(myTimers.size());
		}

		void TimerManager::UpdateTimers()
		{
			QueryPerformanceCounter(&myTick1);

			Time deltaTime(myFrequency.QuadPart);
			deltaTime.SetData(((myTick1.QuadPart - myTick2.QuadPart))); // (myFrequency.QuadPart));
			myMasterTimer.Update(deltaTime);

			for (size_t i = 0; i < myTimers.size(); i++)
			{
				myTimers[i].Update(deltaTime);
			}

			myTick2 = myTick1;
		}
	}
}