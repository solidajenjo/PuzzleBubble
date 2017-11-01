#pragma once
class WaitTimer
{
public:
	WaitTimer();
	WaitTimer(int amount);
	void update(int deltaTime);
	bool isFinished();
	~WaitTimer();
private:
	int timer = 0;
};

