#ifndef LOG_TAG
#define DBG_TAG               "drv"
#else
#define DBG_TAG               LOG_TAG
#endif

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif

#include <rtdbg.h>
