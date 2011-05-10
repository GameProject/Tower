#ifndef TIME_H
#define TIME_H

#include <d3dx10.h>

class Time
{
private:
	LARGE_INTEGER freq, old, current;
	DWORD start, cur;

public:
	Time();
	float getGameTime();
	float getDeltaTime();
};
Time& GetTime();
#endif