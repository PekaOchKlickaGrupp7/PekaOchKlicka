#pragma once
#include "Timer.h"
#include <vector>
#include <Windows.h>

namespace CU
{
	namespace TimeSys
	{
		class TimerManager
		{
		public:
			TimerManager();
			~TimerManager();

			const Timer& GetMasterTimer() const;
			const char GetCountOfTimers() const;
			const Timer& GetTimer(unsigned char ID) const; // We'll use unsigned char since it only takes 1 byte. 
			// and we won't have more than 255 timers
			unsigned char CreateTimer();
			void UpdateTimers();
			void PauseTimers();
			void StartTimers(unsigned char ID);
			void StopTimers(unsigned char ID);
			double GetElapsedTime();
		private:
			void Init();
			Timer myMasterTimer;
			std::vector<Timer> myTimers;

			LARGE_INTEGER myFrequency;
			LARGE_INTEGER myTick1, myTick2;
			LARGE_INTEGER myStartTime;
			double myElapsedTime;
		};
	}
}