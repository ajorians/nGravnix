#include "Timer.h"

Timer::Timer()
{
   Start();
}

void Timer::Start()
{
   m_nStart = GetTime();
}

void Timer::Stop()
{
   m_nElapsed = GetTime() - m_nStart;
}

int Timer::GetElapsed() const
{
   return m_nElapsed;
}

int Timer::GetTime()
{
   return * (volatile unsigned*) 0x90090000;
}

