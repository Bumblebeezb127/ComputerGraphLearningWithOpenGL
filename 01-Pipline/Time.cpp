#include "Time.h"

void Time::setStartTime()
{
	startTime = clock();
}

void Time::setEndTime()
{
	endTime = clock();
	setDeltTime();
	setFps();
}
clock_t Time::getDeltTime()
{
    return deltTime;
}

float Time::getFps()
{
    return fps;
}

void Time::setDeltTime()
{
	deltTime = endTime - startTime;
}

void Time::setFps()
{
	fps = 1000.0f / deltTime;
}