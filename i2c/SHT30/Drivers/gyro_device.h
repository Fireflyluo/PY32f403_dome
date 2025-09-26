/*-----------------------------------------------File Info------------------------------------------------
** File Name:               gyro_driver.c  
** Last modified date:      2025.8.18
** Last version:            V0.1
** Description:             ������ͳһ�ӿ�
**
**--------------------------------------------------------------------------------------------------------            
** Created date:            2025.7.1
** author:                  Fireflyluo
** Version:                 V0.1
** Descriptions:            ����ͳһ�Ĵ������ӿڣ�����Ӳ�����졣
**                                             
**--------------------------------------------------------------------------------------------------------*/


#ifndef GYRO_DEVICE_H
#define GYRO_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* ����������ö�� */
typedef enum {
    GYRO_RANGE_250DPS = 0,
    GYRO_RANGE_500DPS,
    GYRO_RANGE_1000DPS,
    GYRO_RANGE_2000DPS
} gyro_range_t;

/* ���ٶȼ�����ö�� (����) */
typedef enum {
    ACCEL_RANGE_2G = 0,
    ACCEL_RANGE_4G,
    ACCEL_RANGE_8G,
    ACCEL_RANGE_16G
} accel_range_t;

/* ���������ö�� */
typedef enum {
    GYRO_ODR_100HZ = 0,
    GYRO_ODR_200HZ,
    GYRO_ODR_400HZ,
    GYRO_ODR_800HZ
} gyro_odr_t;

/* �������������� */
typedef struct {
    float accel[3];  // ���ٶ� (m/s2)
    float gyro[3];   // ���ٶ� (rad/s)
    float temp;      // �¶� (��C)
} sensor_data_t;

/* ͳһ�������ӿڽṹ */
typedef struct {
    int32_t (*init)(void *hardware_handle);      // ��ʼ���豸
    int32_t (*read_data)(sensor_data_t *data); // ��ȡ����������
    int32_t (*set_range)(gyro_range_t range);   // ��������
    int32_t (*set_odr)(gyro_odr_t odr);        // �������������
    int32_t (*sleep)(void);                    // ����͹���ģʽ
    int32_t (*wakeup)(void);                   // �����豸
} gyro_device_t;

#ifdef __cplusplus
}

#endif

#endif /* GYRO_DEVICE_H */
