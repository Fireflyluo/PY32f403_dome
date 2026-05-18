#include <sys/time.h>
#include <reent.h>
#include <time.h>
#include <rtthread.h>

extern char __bss_end; /* defined in link.lds */
static char *heap_end = &__bss_end;

int _gettimeofday(struct timeval *tv, void *tz)
{
    if (tv)
    {
        tv->tv_sec = 0;
        tv->tv_usec = 0;
    }
    return 0;
}

time_t timegm(struct tm *tm)
{
    time_t t = 0;
    if (tm)
        t = mktime(tm);
    return t;
}

void *_sbrk(int incr)
{
    char *prev_heap = heap_end;
    heap_end += incr;
    if ((rt_uint32_t)heap_end > 0x20010000UL)
        return (void *)-1;
    return prev_heap;
}
