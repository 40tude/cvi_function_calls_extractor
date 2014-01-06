#include <windows.h>
#include "hr_time.h"

// ----------------------------------------------------------------------------
void StartTimer(StopWatch *timer) {
  QueryPerformanceCounter(&timer->start);
}

// ----------------------------------------------------------------------------
void StopTimer(StopWatch *timer) {
  QueryPerformanceCounter(&timer->stop);
}

// ----------------------------------------------------------------------------
double LIToSecs( LARGE_INTEGER * L) {
  
  LARGE_INTEGER frequency;
  
  QueryPerformanceFrequency( &frequency );
  return ((double)L->QuadPart /(double)frequency.QuadPart);
}

// ----------------------------------------------------------------------------
double GetElapsedTime(StopWatch *timer) {
  
  LARGE_INTEGER time;
  
  time.QuadPart = timer->stop.QuadPart - timer->start.QuadPart;
  return LIToSecs( &time) ;
}
