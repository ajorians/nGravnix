#ifndef TIMER_H
#define TIMER_H

extern "C"
{
        #include <os.h>
}

class Timer
{
public:
   Timer();
   void Start();
   void Stop();
   int GetElapsed() const;

protected:
   int GetTime();

protected:
   int m_nStart;
   int m_nElapsed;
};

#endif
