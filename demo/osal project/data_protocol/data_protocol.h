#ifndef DATA_PROTOCOL_H
#define DATA_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

// ==================== 配置选项 ====================
#ifndef PROTOCOL_MAX_FRAME_LEN
#define PROTOCOL_MAX_FRAME_LEN 256
#endif

#ifndef PROTOCOL_BUFFER_SIZE
#define PROTOCOL_BUFFER_SIZE 512
#endif

// 平台抽象
#ifdef __GNUC__
#define PACKED __attribute__((packed))
#else
#define PACKED
#pragma pack(push, 1)
#endif

// ==================== 数据类型定义 ====================
// 消息类型枚举
typedef enum {
  MSG_UNKNOWN = 0,

  // 主机到设备的消息
  MSG_QUERY = 'S',        // 查询消息
  MSG_REPEAT_QUERY = 'E', // 重复查询
  MSG_SET_PARAMS = 'P',   // 设置参数

  // 设备到主机的消息
  MSG_REPORT_FIRST = 'i',  // 检测信息（第一部分）
  MSG_REPORT_MIDDLE = 'm', // 检测信息（传输中）
  MSG_REPORT_LAST = 'h',   // 检测信息（最后部分）
  MSG_REPORT_NONE = 'n',   // 无检测信息
  MSG_ACK_PARAMS = 'q',    // 确认设置参数
} msg_type_t;

// 帧状态枚举
typedef enum {
  FRAME_OK = 0,                 // 成功
  FRAME_ERR_INVALID_START = -1, // 无效的起始标志
  FRAME_ERR_INVALID_END = -2,   // 无效的结束标志
  FRAME_ERR_CRC = -3,           // CRC校验失败
  FRAME_ERR_TOO_LONG = -4,      // 帧过长
  FRAME_ERR_INVALID_CHAR = -5,  // 无效的Base64字符
  FRAME_ERR_INCOMPLETE = -6     // 不完整的帧
} frame_status_t;

// 消息帧结构定义（不使用）
typedef struct PACKED {
  char start_mark;     // 引导码 '#'
  msg_type_t msg_type; // 消息名
  char seq_char;       // 消息号（Base64编码）
  char *content;       // 可变内容
  char crc[4];         // CRC校验码（4个Base64字符）
  char end_mark;       // 结束码 '$'

} frame_header_t;

// 解码后的数据结构
typedef struct {
  uint8_t tag_id[5];      // 36位标签ID（5字节）
  uint8_t start_hour;     // 时
  uint8_t start_minute;   // 分
  uint8_t start_second;   // 秒
  uint16_t sequence;      // 序号
  uint16_t temperature;   // 温度 * 10
  uint16_t humidity;      // 湿度 * 10
  uint16_t *acceleration; // 加速度数组
  uint16_t accel_count;   // 加速度数据点数
} report_data_decoded_t;

// 设置参数数据结构（解码后）
typedef struct {
  uint32_t T1;             // 时间参数1（毫秒）
  uint32_t T2;             // 时间参数2（毫秒）
  uint32_t T3;             // 时间参数3（毫秒）
  uint32_t T4;             // 时间参数4（毫秒）
  uint16_t threshold_high; // 高门限
  uint16_t threshold_low;  // 低门限
  uint8_t master_time[5];  // 主设备时间（36位）
} param_data_decoded_t;

#ifndef __GNUC__
#pragma pack(pop)
#endif

// ==================== Base64编解码 ====================
const char *get_base64_table(void);
uint16_t base64_encode(const uint8_t *input, uint16_t input_len, char *output);
int16_t base64_decode(const char *input, uint16_t input_len, uint8_t *output);

// ==================== CRC24Q校验 ====================
uint32_t crc24q_calculate(const uint8_t *data, uint16_t length);
void crc_to_base64(uint32_t crc, char *output);
uint32_t crc_from_base64(const char *input);

// ==================== 消息帧处理 ====================
uint16_t build_frame(char *frame, uint16_t frame_size, msg_type_t type,
                     uint8_t seq, const char *content, uint16_t content_len);

frame_status_t parse_frame(const char *frame, uint16_t frame_len,
                           msg_type_t *type, uint8_t *seq, char *content_buf,
                           uint16_t *content_len, uint32_t *crc_received);

bool is_frame_complete(const char *frame, uint16_t frame_len,
                       uint16_t *frame_start, uint16_t *frame_end);

// ==================== 消息构建函数 ====================
uint16_t build_query_msg(char *buffer, uint16_t buffer_size, uint8_t seq);
uint16_t build_repeat_query_msg(char *buffer, uint16_t buffer_size,
                                uint8_t seq);
uint16_t build_set_params_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                              const param_data_decoded_t *params);
uint16_t build_report_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                          msg_type_t type, const report_data_decoded_t *data);
uint16_t build_ack_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                       const char *original_content, uint16_t content_len);

// ==================== 消息解析函数 ====================
bool parse_report_data(const char *encoded_data, uint16_t data_len,
                       report_data_decoded_t *report);
bool parse_param_data(const char *encoded_data, uint16_t data_len,
                      param_data_decoded_t *params);

// ==================== 数据转换函数 ====================
uint16_t temperature_to_12bit(float temp_celsius);
float temperature_from_12bit(uint16_t data);
uint16_t humidity_to_12bit(float humidity);
float humidity_from_12bit(uint16_t data);
uint16_t acceleration_to_12bit(float accel_g, float full_scale);
float acceleration_from_12bit(uint16_t data, float full_scale);

// ==================== 调试和日志 ====================
void hex_dump(const uint8_t *data, uint16_t length);
void print_frame(const char *frame, uint16_t length);
void print_report_data(const report_data_decoded_t *report);
void print_param_data(const param_data_decoded_t *params);

#endif // DATA_PROTOCOL_H