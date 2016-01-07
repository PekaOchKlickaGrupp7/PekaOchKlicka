#pragma once
#include "Time.h"

namespace CU
{
	namespace TimeSys
	{
		class Timer
		{
		public:
			Timer();
			~Timer();

			int GetFPS() const;
			void SetFrequency(long long &aFrequency);
			Time GetTime() const;
			Time GetTimeElapsed() const;
			void Start(Time aTimeStamp);
			void Stop();
			void Pause();
			void Update(Time aTimeStamp);
		private:
			// FPS calculating
			int myCounter = 0;
			int myToCheck = 1;
			int myFPS = 0;
			// eof fps calc

			long long myElapsedTime;
			long long myTotalElapsedTime;
			long long myFrequency;
			bool isStopped;
		};
	}
}