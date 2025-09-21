/*-----------------------------------------------File Info------------------------------------------------
** File Name:               gyro_factory.c  
** Last modified date:      2025.8.18
** Last version:            V0.1
** Description:             传感器工厂
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            传感器工厂模式，管理多传感器实例的创建和切换。
**                                             
**--------------------------------------------------------------------------------------------------------*/


#include "gyro_factory.h"
#include "qmi8658a_driver.h"
#include <stdio.h>
// #include "LSM6DSO_driver.h"  // 其他传感器驱动

/* 当前选择的传感器 */
static const gyro_device_t *current_sensor = NULL;

/* 初始化传感器工厂 */
int32_t gyro_factory_init(gyro_sensor_type_t type, void *hardware_handle)
{
    switch(type) {
        case GYRO_QMI8658A:
            current_sensor = &qmi8658a_device;
            return qmi8658a_init(hardware_handle);
        /*
        case GYRO_LSM6DSO:
            current_sensor = &LSM6DSO_device;
            return LSM6DSO_init(hardware_handle);
        */
        default:
            return -1; // 不支持的传感器类型
    }
}

/* 获取当前传感器接口 */
const gyro_device_t *gyro_factory_get_device(void)
{
    return current_sensor;
}

/* 切换传感器类型 */
int32_t gyro_factory_switch_sensor(gyro_sensor_type_t new_type)
{
    // 先休眠当前传感器
    if (current_sensor && current_sensor->sleep) {
        current_sensor->sleep();
    }
    
    // 初始化新传感器
    return gyro_factory_init(new_type, NULL);
}
