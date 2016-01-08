#include "stdafx.h"
#include "Time.h"

namespace CU
{
	namespace TimeSys
	{
		Time::Time(long long aFrequency)
		{
			myFrequency = aFrequency;
		}


		Time::~Time()
		{
		}

		void Time::SetData(long long aData)
		{
			MyTime = aData;
		}

		const double Time::GetMicroseconds() const
		{
			return (static_cast<double>(MyTime)) / myFrequency;
		}

		const double Time::GetMiliseconds() const
		{
			return (static_cast<double>(MyTime) / 1000.0) / myFrequency;
		}

		const double Time::GetSeconds() const
		{
			return (static_cast<double>(MyTime) / 1000000.0) / myFrequency;
		}

		const double Time::GetMinutes() const
		{
			return (((static_cast<double>(MyTime) / 1000000.0) / 60.0) / myFrequency);
		}

		const double Time::GetHours() const
		{
			return ((((static_cast<double>(MyTime) / 1000000.0) / 60.0) / 60.0) / myFrequency);
		}

		const long long Time::GetMicrosecondsAsLongLong() const
		{
			return MyTime;
		}
	}
}
