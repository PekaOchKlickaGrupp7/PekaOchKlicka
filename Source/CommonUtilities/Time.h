#pragma once

namespace CU
{
	namespace TimeSys
	{
		class Time
		{
		public:
			Time(long long aFrequency);
			~Time();

			void SetData(long long aData);
			const double GetHours() const;
			const double GetMinutes() const; // Make everything const because const is a great principle and it's awesome
			const double GetSeconds() const;
			const double GetMiliseconds() const;
			const double GetMicroseconds() const;
			const long long GetMicrosecondsAsLongLong() const;

		private:
			long long MyTime; // In microseconds - gets converted in each return
			long long myFrequency;
		};
	}
}
