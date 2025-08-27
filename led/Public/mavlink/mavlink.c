#include "mavlink.h"
#include "mavlink_types.h"

#include "uart.h"
#include "ringbuffer.h"
#include "stdio.h"


#define  MAVLINK_READ_BUFFER_SIZE 128
/*缓冲区管理器*/
//ringbuffer管理变量
RingBuffer  m_Mavlink_RX_RingBuffMgr;
/*MAVLINK接收数据缓冲区数组*/
uint8_t   m_Mavlink_RX_Buff[MAVLINK_READ_BUFFER_SIZE*10];
uint8_t mavlink_byte;


/*在“mavlink_helpers.h中需要使用”*/
mavlink_system_t mavlink_system = {
    .sysid = 1,    // 系统ID
    .compid = 1    // 组件ID
};

/**
 * @brief Mavlink接收
 * 此函数关联底层硬件接口，如启用串口+DMA接收
 */
void Mavlink_Rece_Enable(void)
{
    //Hal_Uart_Receive_DMA(&huart1,&mavlink_byte,1);
}

/**
 * @brief Mavlink发送字节
 *  * 此函数关联底层硬件接口，如启用串口发送
 * @param chan 通道参数，在MAVLink协议中指定，本函数中未使用该参数
 * @param ch 指向要发送的字节的常量指针
 * @param length 要发送的字节数
 */
void mavlink_send_uart_bytes(mavlink_channel_t chan, const uint8_t *ch, int length)
{
//    HAL_UART_Transmit(&huart8, (uint8_t *)ch, length, 2000);
	Serial_SendArray((uint8_t *)ch, length);
}

/**
 * @brief 初始化Mavlink环形缓冲区
 */
void Mavlink_RB_Init(void)
{
    // 初始化Mavlink接收环形缓冲区管理器，绑定缓冲区地址并设定缓冲区大小
    RingBuffer_Init(&m_Mavlink_RX_RingBuffMgr, m_Mavlink_RX_Buff,sizeof(m_Mavlink_RX_Buff));
}

/**
 * @brief Mavlink_Init初始化函数
 * 
 * 本函数负责初始化Mavlink相关的组件，为Mavlink协议的数据接收和处理做好准备
 * 它首先初始化接收缓冲区，然后启用Mavlink数据接收功能
 * 
 * @param pRingBuff 环形缓冲区的指针
 * @param value 要写入缓冲区的数据值
 */
void Mavlink_Init(void)
{
    // 初始化Mavlink接收缓冲区
    Mavlink_RB_Init(); 
    // 启用Mavlink数据接收
    Mavlink_Rece_Enable();
}

/**
 * @brief 清空Mavlink接收环形缓冲区
 */
void Mavlink_RB_Clear(void)
{
    RingBuffer_Clear(&m_Mavlink_RX_RingBuffMgr);
}

/**
 * @brief  检查Mavlink接收环形缓冲区是否溢出
 * 
 * 此函数用于判断Mavlink接收环形缓冲区是否有数据溢出
 * 
 * @return uint8_t 返回过载标志位的状态
 *         溢出为1，反之为0
 */
uint8_t  Mavlink_RB_IsOverFlow(void)
{
    return m_Mavlink_RX_RingBuffMgr.flagOverflow;
}

/**
 * @brief 向环形缓冲区中添加一个字节的数据
 * 
 * 将接收的数据压入缓冲区
 * 
 * @param data 要添加到环形缓冲区中的数据字节。
 */
void Mavlink_RB_Push(uint8_t data)
{
    RingBuffer_Push(&m_Mavlink_RX_RingBuffMgr, (uint8_t)(data & (uint8_t)0xFFU));
}

/**
 * @brief 向环形缓冲区中取出一个字节的数据
 * 
 * 从缓冲器读出数据
 * @return uint8_t 读出的数据
 */
uint8_t Mavlink_RB_Pop(void)
{
    return RingBuffer_Pop(&m_Mavlink_RX_RingBuffMgr);
}

/**
 * @brief 检查MAVLink环形缓冲区是否为空
 * @return 0表示没有新数据，非0表示有新数据
 */
uint8_t Mavlink_RB_HasNew(void)
{
    // 返回环形缓冲区是否为空的反面，即是否有新数据
    return !RingBuffer_IsEmpty(&m_Mavlink_RX_RingBuffMgr);
}

/**
 * @brief 获取Mavlink接收环形缓冲区中的数据计数
 * 
 * 判断有多少个新数据
 * 
 * @return 缓冲区中的数据计数
 */
uint16_t Mavlink_RB_Count(void)
{
    return RingBuffer_GetCount(&m_Mavlink_RX_RingBuffMgr);
}


/**
 * @brief Mavlink接收中断处理函数
 * 
 * 串口中断的处理函数，主要是讲数据压入ringbuffer管理器
 */
void Mavlin_RX_InterruptHandler(void)
{
    //数据压入
    RingBuffer_Push(&m_Mavlink_RX_RingBuffMgr,mavlink_byte);
}

/**
 * @brief 处理 MAVLink 消息，根据消息 ID 执行相应操作。
 * 
 * 处理从QGC上位机传过来的数据信息
 * 
 * @param msg 需要处理的 MAVLink 消息。
 */
void Mavlink_Msg_Handle(mavlink_message_t msg)
{
    // 根据消息 ID 确定 MAVLink 消息类型，并执行相应的操作
    switch(msg.msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
        // 处理心跳消息，通常用于设备状态更新或连接维护
        printf("this is heartbeat from QGC/r/n");
        break;
    case MAVLINK_MSG_ID_SYS_STATUS:
        // 处理系统状态消息，通常包含电池状态等信息
        // 下面的代码被注释掉了，可以用来获取电池电压、电流和剩余电量
        /*
        osd_vbat = (mavlink_msg_sys_status_get_voltage_battery(&msg) / 1000.0f); // 电池电压，单位为毫伏（1 = 1 毫伏）
        osd_curr = mavlink_msg_sys_status_get_current_battery(&msg); // 电池电流，单位为 10*毫安（1 = 10 毫安）
        osd_battery_remaining = mavlink_msg_sys_status_get_battery_remaining(&msg); // 剩余电池能量：（0%: 0, 100%: 100）
        */
        break;
    default:
        // 默认情况，目前无操作
        break;
    }
}

mavlink_message_t msg;
mavlink_status_t status;

/**
 * @brief 持续解析Mavlink消息的循环函数
 * 在main函数中不间断调用此函数
 * 该函数检查接收缓冲区中是否有待处理的数据，如果有，则进行解析。
 * 如果解析成功，则调用处理函数来处理接收到的消息。
 */
void Loop_Mavlink_Parse(void)
{
    // 检查接收缓冲区是否溢出，如果溢出则清空缓冲区
    if(Mavlink_RB_IsOverFlow())
    {
        Mavlink_RB_Clear();
    }

    // 当接收缓冲区中有新数据时，循环解析
    while(Mavlink_RB_HasNew())
    {
        // 从接收缓冲区中弹出一个字节的数据
        uint8_t read = Mavlink_RB_Pop();
        // 解析Mavlink消息字符，如果解析成功则调用处理函数
        if(mavlink_parse_char(MAVLINK_COMM_0, read, &msg, &status))
        {
            // 信号处理函数
            Mavlink_Msg_Handle(msg);
            // 打印接收到的消息信息，包括消息ID、序列号、组件ID和系统ID
            // printf("Received message with ID %d, sequence: %d from component %d of system %d", msg.msgid, msg.seq, msg.compid, msg.sysid);
        }
    }
}




/*测试函数代码区******************************************************************************/

/**
 * @brief 测试发送心跳消息 (Heartbeat Message)
 *
 * 此函数用于测试发送 MAVLink 心跳消息。它会根据给定的系统 ID 和组件 ID 构建并发送一个心跳消息，
 * 并验证发送的消息是否与预期一致。
 *
 * @param system_id 系统 ID
 * @param component_id 组件 ID
 * @param last_msg 上一次接收到的消息（用于状态检查）
 */
static void mavlink_test_heartbeat2(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    // 获取通道 0 的状态
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
    
    // 如果是 MAVLink 1.0 协议且心跳消息 ID 大于等于 256，则直接返回
    if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_HEARTBEAT >= 256)
    {
        return;
    }
#endif

    // 创建一个新的 MAVLink 消息对象和缓冲区
    mavlink_message_t msg;
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t i;

    // 初始化心跳消息的数据结构
    mavlink_heartbeat_t packet_in =
    {
        963497464, 17, 84, 151, 218, 3
    };

    // 创建两个心跳消息数据结构，packet1 用于构建消息，packet2 用于接收发送后的消息
    mavlink_heartbeat_t packet1, packet2;
    memset(&packet1, 0, sizeof(packet1));

    // 将 packet_in 的内容复制到 packet1
    packet1.custom_mode = packet_in.custom_mode;
    packet1.type = packet_in.type;
    packet1.autopilot = packet_in.autopilot;
    packet1.base_mode = packet_in.base_mode;
    packet1.system_status = packet_in.system_status;
    packet1.mavlink_version = packet_in.mavlink_version;

#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    // 如果是 MAVLink 1.0 协议，处理扩展字段
    if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
    {
        memset((char *)&packet1 + MAVLINK_MSG_ID_HEARTBEAT_MIN_LEN, 0, sizeof(packet1) - MAVLINK_MSG_ID_HEARTBEAT_MIN_LEN);
    }
#endif

    // 清空 packet2
    memset(&packet2, 0, sizeof(packet2));

    // 发送心跳消息
    mavlink_msg_heartbeat_send(MAVLINK_COMM_0, packet1.type, packet1.autopilot, packet1.base_mode, packet1.custom_mode, packet1.system_status);

    // 断言：发送的消息应与原始消息一致
    MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

// extern MPU6000_Data 		m_Mpu6000;
// extern MS56XX_Data 	  m_Ms56xx;
// extern hmc5883MagData 	m_Hmc5883;
// /**
//  * @brief 发送 MAVLink RAW_IMU 消息
//  * 
//  * 该函数用于封装并发送来自 MPU6000 和 HMC5883 传感器的原始 IMU 数据。
//  * 
//  * @param system_id 系统 ID
//  * @param component_id 组件 ID
//  * @param last_msg 上一次的消息（未使用）
//  */
// static void mavlink_test_raw_imu2(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
// {
//     // 检查是否需要兼容 MAVLink 1.0 协议
// #ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
//     mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
//     if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_RAW_IMU >= 256)
//     {
//         return;
//     }
// #endif

//     // 初始化消息和缓冲区
//     mavlink_message_t msg;
//     uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
//     uint16_t i;

//     // 创建并填充 RAW_IMU 数据包
//     mavlink_raw_imu_t packet1;
//     memset(&packet1, 0, sizeof(packet1));

//     // 设置时间戳
//     packet1.time_usec = 123456;  // 时间戳（微秒）

//     // 填充加速度计数据
//     packet1.xacc = m_Mpu6000.acc_raw.x;
//     packet1.yacc = m_Mpu6000.acc_raw.y;
//     packet1.zacc = m_Mpu6000.acc_raw.z;

//     // 填充陀螺仪数据
//     packet1.xgyro = m_Mpu6000.gyro_raw.x;
//     packet1.ygyro = m_Mpu6000.gyro_raw.y;
//     packet1.zgyro = m_Mpu6000.gyro_raw.z;

//     // 填充磁力计数据
//     packet1.xmag = (int16_t)m_Hmc5883.x;
//     packet1.ymag = (int16_t)m_Hmc5883.y;
//     packet1.zmag = (int16_t)m_Hmc5883.z;

//     // 处理 MAVLink 1.0 兼容性
// #ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
//     if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1)
//     {
//         // 处理扩展字段
//         memset(MAVLINK_MSG_ID_RAW_IMU_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_RAW_IMU_MIN_LEN);
//     }
// #endif

//     // 发送 RAW_IMU 消息
//     mavlink_msg_raw_imu_send(MAVLINK_COMM_0, packet1.time_usec, packet1.xacc, packet1.yacc, packet1.zacc, 
//                              packet1.xgyro, packet1.ygyro, packet1.zgyro, packet1.xmag, packet1.ymag, packet1.zmag);
// }

/**
 * @brief MAVLink测试函数
 * 
 * 该函数用于周期性地发送MAVLink测试消息。它以不同的频率发送心跳和原始IMU数据。
 * 心跳信号每20秒发送一次（5Hz），而原始IMU数据每10秒发送一次（10Hz）。
 * 这种设计确保了系统既能定期更新其状态，又不会过度占用传输带宽。
 */
void mavlink_test(void)
{
    // 定义一个静态变量来计数函数被调用的次数
    static uint16_t test_count=0;
    // 定义一个mavlink_message_t变量来存储最后的消息
    mavlink_message_t lastmsg;
    // 每次函数被调用时，计数器增加
    test_count++;
    // 5hz
    // 当计数器达到100的倍数时，发送心跳测试消息
    if((test_count%100)==0)
    {
        mavlink_test_heartbeat2(1,1,&lastmsg);
    }
    // 当计数器达到50的倍数时，发送原始IMU数据测试消息
    // if((test_count%50)==0)
    // {
    //     mavlink_test_raw_imu2(1,1,&lastmsg);
    // }
}
