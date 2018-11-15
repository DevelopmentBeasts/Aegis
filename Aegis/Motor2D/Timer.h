#ifndef TIMER_H_
#define TIMER_H_

#include "p2Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	float Read() const;
	float ReadSec() const;

private:
	float	started_at;
};

#endif //__j1TIMER_H__