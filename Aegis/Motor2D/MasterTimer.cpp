// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "MasterTimer.h"
#include "SDL\include\SDL_timer.h"

double MasterTimer::frequency = 0;

// ---------------------------------------------
MasterTimer::MasterTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void MasterTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double MasterTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
double MasterTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}


