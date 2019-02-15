
#pragma once
#include<cstdint>



class Timer
{
public:

	Timer();
	~Timer();
	
	float GetTotalTime() const;
	float GetDeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:

	double SecondsPerCount;
	double DeltaTime;

	uint64_t BaseTime;
	uint64_t PausedTime;
    uint64_t StopTime;
	uint64_t PrevTime;
	uint64_t CurrTime;

	bool Stopped;


};