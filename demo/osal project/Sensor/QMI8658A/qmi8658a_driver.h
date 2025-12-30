/**
 ******************************************************************************
 * @file    qmi8658a_driver.h
 * @brief   QMI8658A IMU传感器驱动头文件
 ******************************************************************************
 */

#ifndef QMI8658A_DRIVER_H
#define QMI8658A_DRIVER_H

#include "qmi8658a_reg.h"
#include "py32f4xx_hal.h"
#ifdef __cplusplus
extern "C" {
#endif
/* 传感器数据类型 */
typedef struct {
    float accel[3];  // 加速度 (m/s2) 
    float gyro[3];   // 角速度 (rad/s)
    float temp;      // 温度 (°C)
    uint32_t timestamp;  // 时间戳 (ms)
    uint8_t sequence;   // 数据序列号
} sensor_data_t;

#define SENSOR_DATA_SIZE sizeof(sensor_data_t) // 33字节
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

/* 设备上下文结构体扩展 */
typedef struct {
    qmi8658a_ctx_t reg_ctx;          // 寄存器操作上下文
    gyro_range_t current_gyro_range;  // 当前陀螺仪量程
    qmi8658a_accel_fs_t current_accel_fs; // 当前加速度计量程
} qmi8658a_driver_ctx_t;
/* 统一传感器接口结构 */
typedef struct {
    int32_t (*init)(void *hardware_handle);      // 初始化设备
    int32_t (*read_data)(sensor_data_t *data); // 读取传感器数据
    int32_t (*set_range)(gyro_range_t range);   // 设置量程
    int32_t (*set_odr)(gyro_odr_t odr);        // 设置输出数据率
    int32_t (*sleep)(void);                    // 进入低功耗模式
    int32_t (*wakeup)(void);                   // 唤醒设备
} gyro_device_t;

/* 导出统一接口 */
extern const gyro_device_t qmi8658a_device;
int32_t qmi8658a_init(void *hardware_handle);
#ifdef __cplusplus
}
#endif

#endif /* QMI8658A_DRIVER_H */
