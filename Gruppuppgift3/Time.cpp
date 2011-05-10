#include "Time.h"

Time::Time()
{
	QueryPerformanceFrequency (&freq);
	QueryPerformanceCounter (&old);
	start = 0;
}

Time& GetTime()
{
	static Time time;
	return time;
}

float Time::getGameTime()
{
	cur = GetTickCount();
	if(start == 0)
		start = cur;
	return (cur - start) / 1000.0f;
}

float Time::getDeltaTime()
{
	QueryPerformanceCounter(&current);
	// get delta then divide double delta (ticks) by frequency
	float dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);
	// copy current counter to old for next frame
	old.QuadPart = current.QuadPart;
	return dt;
}