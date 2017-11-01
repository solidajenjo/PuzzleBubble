#include "WaitTimer.h"



WaitTimer::WaitTimer()
{
}

WaitTimer::WaitTimer(int amount)
{
	this->timer = amount;
}

void WaitTimer::update(int deltaTime)
{
	timer -= deltaTime;
	if (timer <= 0) timer = 0;
}

bool WaitTimer::isFinished()
{
	return (timer == 0);
}

WaitTimer::~WaitTimer()
{
}
