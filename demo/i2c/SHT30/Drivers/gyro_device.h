/*-----------------------------------------------File Info------------------------------------------------
** File Name:               gyro_driver.c  
** Last modified date:      2025.8.18
** Last version:            V0.1
** Description:             传感器统一接口
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            定义统一的传感器接口，屏蔽硬件差异。
**                                             
**--------------------------------------------------------------------------------------------------------*/


#ifndef GYRO_DEVICE_H
#define GYRO_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* 传感器量程枚举 */
typedef enum {
    GYRO_RANGE_250DPS = 0,
    GYRO_RANGE_500DPS,
    GYRO_RANGE_1000DPS,
    GYRO_RANGE_2000DPS
} gyro_range_t;

/* 加速度计量程枚举 (新增) */
typedef enum {
    ACCEL_RANGE_2G = 0,
    ACCEL_RANGE_4G,
    ACCEL_RANGE_8G,
    ACCEL_RANGE_16G
} accel_range_t;

/* 输出数据率枚举 */
typedef enum {
    GYRO_ODR_100HZ = 0,
    GYRO_ODR_200HZ,
    GYRO_ODR_400HZ,
    GYRO_ODR_800HZ
} gyro_odr_t;

/* 传感器数据类型 */
typedef struct {
    float accel[3];  // 加速度 (m/s2)
    float gyro[3];   // 角速度 (rad/s)
    float temp;      // 温度 (°C)
} sensor_data_t;

/* 统一传感器接口结构 */
typedef struct {
    int32_t (*init)(void *hardware_handle);      // 初始化设备
    int32_t (*read_data)(sensor_data_t *data); // 读取传感器数据
    int32_t (*set_range)(gyro_range_t range);   // 设置量程
    int32_t (*set_odr)(gyro_odr_t odr);        // 设置输出数据率
    int32_t (*sleep)(void);                    // 进入低功耗模式
    int32_t (*wakeup)(void);                   // 唤醒设备
} gyro_device_t;

#ifdef __cplusplus
}

#endif

#endif /* GYRO_DEVICE_H */
