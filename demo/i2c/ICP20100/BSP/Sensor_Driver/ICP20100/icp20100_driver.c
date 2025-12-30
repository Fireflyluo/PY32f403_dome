/**
 * @file icp20100_driver.c
 * @brief ICP20100传感器驱动层统一接口实现
 * @version 1.0
 * @date 2025
 */

/****************************************************************************/
/**\name        头文件                                                      */
/****************************************************************************/
#include "icp20100_driver.h"

/****************************************************************************/
/**\name        函数定义                                                    */
/****************************************************************************/

/*!
 * @brief 此API使设备进入睡眠模式
 */
int8_t icp20100_sleep(struct icp20100_dev *dev)
{
    /* 检查设备结构体中的空指针 */
    if (dev == NULL) 
    {
        return ICP20100_E_NULL_PTR;
    }
    
    /* 设置为正常电源模式以降低功耗 */
    return icp20100_set_power_mode(ICP20100_POWER_MODE_NORMAL, dev);
}

/*!
 * @brief 此API唤醒设备
 */
int8_t icp20100_wakeup(struct icp20100_dev *dev)
{
    int8_t rslt;
    
    /* 检查设备结构体中的空指针 */
    if (dev == NULL) 
    {
        return ICP20100_E_NULL_PTR;
    }
    
    /* 设置为活动电源模式 */
    rslt = icp20100_set_power_mode(ICP20100_POWER_MODE_ACTIVE, dev);
    
    if (rslt == ICP20100_OK) 
    {
        /* 等待设备上电完成 */
        dev->delay_us(ICP20100_POWER_UP_TIME_MS * 1000, dev->intf_ptr);
    }
    
    return rslt;
}

/*!
 * @brief ICP20100设备操作接口实例
 */
const icp20100_device_ops_t icp20100_device_ops = {
    .init = icp20100_init,
    .read_data = icp20100_get_data,
    .set_config = icp20100_set_config,
    .sleep = icp20100_sleep,
    .wakeup = icp20100_wakeup
};