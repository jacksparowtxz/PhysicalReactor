#include <windows.h>
#include "Timer.h"



Timer::Timer():SecondsPerCount(0.0),DeltaTime(-1.0),BaseTime(0),
       PausedTime(0),PrevTime(0),CurrTime(),Stopped(false)
{
	uint64_t counterPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counterPerSec);
	SecondsPerCount = 1.0F / (double)counterPerSec;
}

Timer::~Timer()
{

}

float Timer::GetTotalTime() const
{
	if (Stopped)
	{
		return (float)(((StopTime - PausedTime) - BaseTime)*SecondsPerCount);
	}
	else
	{
		return (float)(((CurrTime - PausedTime) - BaseTime)*SecondsPerCount);
	}
}

float Timer::GetDeltaTime() const
{
	return (float)DeltaTime;
}

void Timer::Reset()
{
	uint64_t currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	BaseTime = currTime;
	PrevTime = currTime;
	StopTime = 0;
	Stopped = false;
}

void Timer::Start()
{
	uint64_t starttime;
	QueryPerformanceCounter((LARGE_INTEGER*)&starttime);

	if (Stopped)
	{
		PausedTime += (starttime - StopTime);

		PrevTime = starttime;
		StopTime = 0;
		Stopped = false;
	}
}

void Timer::Stop()
{
	if (!Stopped)
	{
		uint64_t currtime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currtime);

		StopTime = currtime;
		Stopped = true;
	}
}

void Timer::Tick()
{
	if (Stopped)
	{
		DeltaTime = 0.0;
		return;
	}

	uint64_t currtime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currtime);
	CurrTime = currtime;

	DeltaTime = (CurrTime - PrevTime)*SecondsPerCount;

	PrevTime = CurrTime;


	if (DeltaTime<0.0)
	{
		DeltaTime = 0.0f;
	}
}

