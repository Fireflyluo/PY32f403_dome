/**
 * @file icp20100_driver.h
 * @brief ICP20100传感器驱动层统一接口
 * @version 1.0
 * @date 2025
 */

#ifndef ICP20100_DRIVER_H_
#define ICP20100_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/**\name        头文件                                                      */
/****************************************************************************/
#include "icp20100.h"

/****************************************************************************/
/**\name        类型定义                                                    */
/****************************************************************************/

/*!
 * @brief ICP20100设备操作函数指针结构体
 */
typedef struct {
    /*! 初始化设备 */
    int8_t (*init)(struct icp20100_dev *dev);
    
    /*! 读取传感器数据 */
    int8_t (*read_data)(struct icp20100_data *data, struct icp20100_dev *dev);
    
    /*! 设置设备配置 */
    int8_t (*set_config)(const struct icp20100_config *config, struct icp20100_dev *dev);
    
    /*! 使设备进入睡眠模式 */
    int8_t (*sleep)(struct icp20100_dev *dev);
    
    /*! 唤醒设备 */
    int8_t (*wakeup)(struct icp20100_dev *dev);
} icp20100_device_ops_t;

/****************************************************************************/
/**\name        函数声明                                                    */
/****************************************************************************/

/*!
 * @brief 此API使设备进入睡眠模式
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_sleep(struct icp20100_dev *dev);

/*!
 * @brief 此API唤醒设备
 *
 * @param[in] dev : icp20100_dev结构体实例
 *
 * @return API执行状态结果
 * @retval ICP20100_OK -> 成功
 * @retval ICP20100_E_NULL_PTR -> 空指针错误
 * @retval ICP20100_E_COMM_FAIL -> 通信失败
 */
int8_t icp20100_wakeup(struct icp20100_dev *dev);

/****************************************************************************/
/**\name        导出设备实例                                                */
/****************************************************************************/

/*! ICP20100设备操作接口实例 */
extern const icp20100_device_ops_t icp20100_device_ops;

#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* ICP20100_DRIVER_H_ */