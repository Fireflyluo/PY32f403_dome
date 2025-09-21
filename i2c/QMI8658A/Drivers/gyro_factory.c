/*-----------------------------------------------File Info------------------------------------------------
** File Name:               gyro_factory.c  
** Last modified date:      2025.8.18
** Last version:            V0.1
** Description:             ����������
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            ����������ģʽ������ഫ����ʵ���Ĵ������л���
**                                             
**--------------------------------------------------------------------------------------------------------*/


#include "gyro_factory.h"
#include "qmi8658a_driver.h"
#include <stdio.h>
// #include "LSM6DSO_driver.h"  // ��������������

/* ��ǰѡ��Ĵ����� */
static const gyro_device_t *current_sensor = NULL;

/* ��ʼ������������ */
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
            return -1; // ��֧�ֵĴ���������
    }
}

/* ��ȡ��ǰ�������ӿ� */
const gyro_device_t *gyro_factory_get_device(void)
{
    return current_sensor;
}

/* �л����������� */
int32_t gyro_factory_switch_sensor(gyro_sensor_type_t new_type)
{
    // �����ߵ�ǰ������
    if (current_sensor && current_sensor->sleep) {
        current_sensor->sleep();
    }
    
    // ��ʼ���´�����
    return gyro_factory_init(new_type, NULL);
}
