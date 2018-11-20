#ifndef __MASTERTIMER_H__
#define __MASTERTIMER_H__

#include "p2Defs.h"

class MasterTimer
{
public:

	// Constructor
	MasterTimer();

	void Start();
	double ReadMs() const;
	double ReadTicks() const;

private:
	double	started_at;
	static double frequency;
};

#endif //__j1PE