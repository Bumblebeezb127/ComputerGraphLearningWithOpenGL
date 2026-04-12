#pragma once
#include<ctime>
class Time {
private:
	clock_t startTime;
	clock_t endTime;
	clock_t deltTime;
	float fps;
public:
	void setStartTime();
	void setEndTime();
	clock_t getDeltTime();
	float getFps();
private:
	void setDeltTime();
	void setFps();
};

