/**
 ******************************************************************************
 * @file    gyro_factory.h
 * @brief   传感器工厂接口
 ******************************************************************************
 */

#ifndef GYRO_FACTORY_H
#define GYRO_FACTORY_H

#include "gyro_device.h"

/* 支持的传感器类型 */
typedef enum {
    GYRO_QMI8658A = 0,
    GYRO_MPU6050,
    GYRO_BMI160,
    GYRO_TYPE_COUNT
} gyro_sensor_type_t;

/* 工厂接口函数 */
int32_t gyro_factory_init(gyro_sensor_type_t type, void *hardware_handle);
const gyro_device_t *gyro_factory_get_device(void);
int32_t gyro_factory_switch_sensor(gyro_sensor_type_t new_type);

#endif /* GYRO_FACTORY_H */
