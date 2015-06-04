#ifndef _TIMER_H
#define _TIMER_H

#include <windows.h>

class Timer
{
    public:
        Timer();
        ~Timer();

        int Refresh();

        double        Time;
        double        lastTime;
        double        frametime;
        int           frames;
        int           fps;
    
        LARGE_INTEGER Frequency;
        double        Resolution;
	
    private:
    	int           lowshift;
        bool          PerformanceTimerEnabled;

        unsigned int  oldTime;
        int           sametimecount;
};

#endif
