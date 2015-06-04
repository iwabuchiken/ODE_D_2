#include <windows.h>
#include "timer.h"

Timer::Timer()
{
    frames = 0;
    frametime = 0;
    Time = 0;
    fps = 0;

    if (!QueryPerformanceFrequency((LARGE_INTEGER *) &Frequency))
    {
        // no performance counter available, you might be using Windows 98 osr1
        Resolution = 0.001f;
        PerformanceTimerEnabled = 0;
    }
    else
    { 
        PerformanceTimerEnabled = 1;

		// get 32 out of the 64 time bits such that we have around
		// 1 microsecond resolution
        unsigned int lowpart = (unsigned int)Frequency.LowPart;
        unsigned int highpart = (unsigned int)Frequency.HighPart;

        lowshift = 0;

        while (highpart || (lowpart > 2000000.0))
        {
            lowshift++;
            lowpart >>= 1;
            lowpart |= (highpart & 1) << 31;
            highpart >>= 1;
        }

        Resolution = 1.0/(double)lowpart;
    }
}

Timer::~Timer()
{
}

int Timer::Refresh()
{
    LARGE_INTEGER   count;
    unsigned int    temp, t2;

    if (PerformanceTimerEnabled)
    {	
        QueryPerformanceCounter((LARGE_INTEGER *) &count);
        
		temp = ((unsigned int)count.LowPart >> lowshift) |
			   ((unsigned int)count.HighPart << (32 - lowshift));

        if (frames == 0 || (temp <= oldTime))
        {
            oldTime = temp;
        }
        else
        {
            t2 = temp - oldTime;
            frametime = (double)t2 * Resolution;
            Time += frametime;

            oldTime = temp;

            if (Time == lastTime)
            {
                sametimecount++;

                if (sametimecount > 100000)
                {
                    Time += 1;
                    sametimecount = 0;
                }
            }
            else sametimecount = 0;

            lastTime = Time;
        }
        frames++;
    }
    else
    {
        if(frames == 0)
        {
            lastTime = Time = timeGetTime() * Resolution;
            frametime = 0;
        }
        else
        {
            lastTime = Time;
            Time = timeGetTime() * Resolution;
            frametime = Time - lastTime;
        }
    }

    fps = int(1.0 / frametime);
    frames++;
    return frames;
}
