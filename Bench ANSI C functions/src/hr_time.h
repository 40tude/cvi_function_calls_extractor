#include <windows.h>

typedef struct {
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
} StopWatch;

void StartTimer(StopWatch *timer);
void StopTimer(StopWatch *timer);
double LIToSecs(LARGE_INTEGER * L);
double GetElapsedTime(StopWatch *timer);
