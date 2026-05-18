#ifndef __ADC_CONFIG_H__
#define __ADC_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BSP_USING_ADC1
#ifndef ADC1_CONFIG
#define ADC1_CONFIG                                                 \
    {                                                               \
       .Instance                   = ADC1,                          \
       .Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4,      \
       .Init.Resolution            = ADC_RESOLUTION_12B,            \
       .Init.DataAlign             = ADC_DATAALIGN_RIGHT,           \
       .Init.ScanConvMode          = DISABLE,                       \
       .Init.EOCSelection          = DISABLE,                       \
       .Init.ContinuousConvMode    = DISABLE,                       \
       .Init.NbrOfConversion       = 1,                             \
       .Init.DiscontinuousConvMode = DISABLE,                       \
       .Init.NbrOfDiscConversion   = 0,                             \
       .Init.ExternalTrigConv      = ADC_SOFTWARE_START,            \
       .Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE, \
       .Init.DMAContinuousRequests = DISABLE,                       \
    }
#endif
#endif

#ifdef BSP_USING_ADC2
#ifndef ADC2_CONFIG
#define ADC2_CONFIG                                                 \
    {                                                               \
       .Instance                   = ADC2,                          \
       .Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4,      \
       .Init.Resolution            = ADC_RESOLUTION_12B,            \
       .Init.DataAlign             = ADC_DATAALIGN_RIGHT,           \
       .Init.ScanConvMode          = DISABLE,                       \
       .Init.EOCSelection          = DISABLE,                       \
       .Init.ContinuousConvMode    = DISABLE,                       \
       .Init.NbrOfConversion       = 1,                             \
       .Init.DiscontinuousConvMode = DISABLE,                       \
       .Init.NbrOfDiscConversion   = 0,                             \
       .Init.ExternalTrigConv      = ADC_SOFTWARE_START,            \
       .Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE, \
       .Init.DMAContinuousRequests = DISABLE,                       \
    }
#endif
#endif

#ifdef BSP_USING_ADC3
#ifndef ADC3_CONFIG
#define ADC3_CONFIG                                                 \
    {                                                               \
       .Instance                   = ADC3,                          \
       .Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4,      \
       .Init.Resolution            = ADC_RESOLUTION_12B,            \
       .Init.DataAlign             = ADC_DATAALIGN_RIGHT,           \
       .Init.ScanConvMode          = DISABLE,                       \
       .Init.EOCSelection          = DISABLE,                       \
       .Init.ContinuousConvMode    = DISABLE,                       \
       .Init.NbrOfConversion       = 1,                             \
       .Init.DiscontinuousConvMode = DISABLE,                       \
       .Init.NbrOfDiscConversion   = 0,                             \
       .Init.ExternalTrigConv      = ADC_SOFTWARE_START,            \
       .Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE, \
       .Init.DMAContinuousRequests = DISABLE,                       \
    }
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
