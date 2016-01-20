#include "stdafx.h"
#include "Timer.h"

namespace CU
{
	namespace TimeSys
	{
		Timer::Timer()
		{
		}


		Timer::~Timer()
		{
		}

		void Timer::SetFrequency(long long &aFrequency)
		{
			myFrequency = aFrequency;
		}

		int Timer::GetFPS() const
		{
			return myFPS;
		}

		Time Timer::GetTime() const
		{
			Time time(myFrequency);
			time.SetData(myTotalElapsedTime);
			return time;
		}

		Time Timer::GetTimeElapsed() const
		{
			Time time(myFrequency);
			time.SetData(myElapsedTime);
			return time;
		}

		void Timer::Start(Time aTimeStamp)
		{
			myElapsedTime = aTimeStamp.GetMicrosecondsAsLongLong();
			myTotalElapsedTime = 0;
			isStopped = false;
		}

		void Timer::Stop()
		{
			isStopped = true;
		}

		void Timer::Update(Time aTimeStamp)
		{
			if (isStopped == false)
			{
				myElapsedTime = aTimeStamp.GetMicrosecondsAsLongLong();
				myTotalElapsedTime += myElapsedTime;
			}

			if (GetTime().GetSeconds() >= myToCheck)
			{
				myFPS = myCounter;
				++myToCheck;
				myCounter = 0;
			}
			else
			{
				++myCounter;
			}
		}

		void Timer::Pause()
		{
			isStopped = true;
		}

		void Timer::Reset()
		{
			myTotalElapsedTime = 0;
		}
	}
}