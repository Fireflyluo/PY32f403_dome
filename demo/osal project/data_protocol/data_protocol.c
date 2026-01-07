#include "data_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==================== Base64编解码 ====================
const char *get_base64_table(void)
{
  static const char table[65] = "0123456789"
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "-_";
  return table;
}

// 编码函数
uint16_t base64_encode(const uint8_t *input, uint16_t input_len, char *output)
{
  const char *table = get_base64_table();
  uint16_t i, j = 0;
  uint8_t octet_a, octet_b, octet_c; // 8位3字节
  uint32_t triple;                   // 组合后的24位

  for (i = 0; i < input_len; i += 3)
  {

    octet_a = input[i];
    octet_b = (i + 1 < input_len) ? input[i + 1] : 0;
    octet_c = (i + 2 < input_len) ? input[i + 2] : 0;

    triple = (octet_a << 16) | (octet_b << 8) | octet_c;

    output[j++] = table[(triple >> 18) & 0x3F];
    output[j++] = table[(triple >> 12) & 0x3F];

    if (i + 1 < input_len)
    {
      output[j++] = table[(triple >> 6) & 0x3F];
    }
    else
    {
      output[j++] = '='; // 填充字符
    }

    if (i + 2 < input_len)
    {
      output[j++] = table[triple & 0x3F];
    }
    else
    {
      output[j++] = '=';
    }
  }

  output[j] = '\0';
  return j;
}
// 解码函数
int16_t base64_decode(const char *input, uint16_t input_len, uint8_t *output)
{
  static int8_t decode_table[256] = {0};
  static bool initialized = false;
  const char *table = get_base64_table();
  // 初始化解码表
  if (!initialized)
  {
    for (int i = 0; i < 256; i++)
    {
      decode_table[i] = -1;
    }
    for (int i = 0; i < 64; i++)
    {
      decode_table[(uint8_t)table[i]] = (int8_t)i;
    }
    decode_table['='] = 0;
    initialized = true;
  }

  if (input_len == 0 || input_len % 4 != 0)
  {
    return -1;
  }

  uint16_t i, j = 0;
  int8_t sextet_a, sextet_b, sextet_c, sextet_d;
  uint32_t triple;
  uint16_t padding = 0;

  // 计算填充字符数量
  if (input_len >= 1 && input[input_len - 1] == '=')
    padding++;
  if (input_len >= 2 && input[input_len - 2] == '=')
    padding++;

  for (i = 0; i < input_len; i += 4)
  {
    sextet_a = decode_table[(uint8_t)input[i]];
    sextet_b = decode_table[(uint8_t)input[i + 1]];
    sextet_c = decode_table[(uint8_t)input[i + 2]];
    sextet_d = decode_table[(uint8_t)input[i + 3]];

    if (sextet_a < 0 || sextet_b < 0 || sextet_c < 0 || sextet_d < 0)
    {
      return -2; // 有无效字符（不在Base64字符表中）
    }
    // 组合24位
    triple = ((uint32_t)sextet_a << 18) | ((uint32_t)sextet_b << 12) |
             ((uint32_t)sextet_c << 6) | (uint32_t)sextet_d;

    output[j++] = (triple >> 16) & 0xFF;
    if (i + 2 < input_len - padding)
    {
      output[j++] = (triple >> 8) & 0xFF;
    }
    if (i + 3 < input_len - padding)
    {
      output[j++] = triple & 0xFF;
    }
  }

  return j;
}
// ==================== CRC24Q校验 ====================
#define CRC24Q_POLY 0x1864CFB
#define CRC24Q_INIT 0x000000
// 计算CRC24Q
uint32_t crc24q_calculate(const uint8_t *data, uint16_t length)
{
  uint32_t crc = CRC24Q_INIT;
  uint16_t i, j;

  for (i = 0; i < length; i++)
  {
    crc ^= ((uint32_t)data[i] << 16);
    for (j = 0; j < 8; j++)
    {
      crc <<= 1;
      if (crc & 0x1000000)
      {
        crc ^= CRC24Q_POLY;
      }
    }
  }

  return crc & 0xFFFFFF;
}
//  CRC编码为Base64字符串
void crc_to_base64(uint32_t crc, char *output)
{
  uint8_t bytes[3];
  bytes[0] = (crc >> 16) & 0xFF;
  bytes[1] = (crc >> 8) & 0xFF;
  bytes[2] = crc & 0xFF;

  base64_encode(bytes, 3, output);
}
// 从Base64字符串解码CRC
uint32_t crc_from_base64(const char *input)
{
  if (strlen(input) != 4)
  {
    return 0xFFFFFFFF; // 错误标志
  }

  uint8_t bytes[3];
  int16_t len = base64_decode(input, 4, bytes);

  if (len == 3)
  {
    return (bytes[0] << 16) | (bytes[1] << 8) | bytes[2];
  }

  return 0xFFFFFFFF;
}

// ==================== 消息帧处理 ====================
// 构建通用消息帧
uint16_t build_frame(char *frame, uint16_t frame_size, msg_type_t type,
                     uint8_t seq, const char *content, uint16_t content_len)
{
  if (frame_size < content_len + 8)
  { // 最小帧长度
    return 0;
  }

  const char *table = get_base64_table();
  uint16_t pos = 0;

  // 引导码
  frame[pos++] = '#';

  // 消息名
  frame[pos++] = (char)type;

  // 消息号（Base64编码）
  if (seq >= 64)
    seq = 0;
  frame[pos++] = table[seq & 0x3F];

  // 消息内容
  if (content != NULL && content_len > 0)
  {
    memcpy(&frame[pos], content, content_len);
    pos += content_len;
  }

  // CRC（从引导码到消息内容）
  uint32_t crc = crc24q_calculate((uint8_t *)frame, pos);

  // CRC校验码
  char crc_str[5];
  crc_to_base64(crc, crc_str);
  memcpy(&frame[pos], crc_str, 4);
  pos += 4;

  // 结束码
  frame[pos++] = '$';

  frame[pos] = '\0';
  return pos;
}
// 解析消息帧
frame_status_t parse_frame(const char *frame, uint16_t frame_len,
                           msg_type_t *type, uint8_t *seq, char *content_buf,
                           uint16_t *content_len, uint32_t *crc_received)
{
  if (frame_len < 7)
  {
    return FRAME_ERR_INCOMPLETE;
  }

  if (frame[0] != '#')
  {
    return FRAME_ERR_INVALID_START;
  }

  if (frame[frame_len - 1] != '$')
  {
    return FRAME_ERR_INVALID_END;
  }

  // 解析消息类型
  *type = (msg_type_t)frame[1];

  // 解析消息号
  const char *table = get_base64_table();
  static int8_t decode_table[256] = {0};
  static bool initialized = false;

  if (!initialized)
  {
    memset(decode_table, -1, sizeof(decode_table));
    for (int i = 0; i < 64; i++)
    {
      decode_table[(uint8_t)table[i]] = i;
    }
    initialized = true;
  }

  *seq = decode_table[(uint8_t)frame[2]];
  if (*seq == 0xFF)
  {
    return FRAME_ERR_INVALID_CHAR;
  }

  // 提取内容
  uint16_t content_start = 3;
  uint16_t content_end = frame_len - 5; // 减去CRC4和$

  if (content_end > content_start)
  {
    *content_len = content_end - content_start;
    if (content_buf)
    {
      memcpy(content_buf, &frame[content_start], *content_len);
    }
  }
  else
  {
    *content_len = 0;
  }

  // 提取CRC
  char crc_str[5];
  memcpy(crc_str, &frame[content_end], 4);
  crc_str[4] = '\0';
  *crc_received = crc_from_base64(crc_str);

  // 计算CRC进行验证
  uint32_t crc_calculated = crc24q_calculate((uint8_t *)frame, content_end);

  if (*crc_received != crc_calculated)
  {
    return FRAME_ERR_CRC;
  }

  return FRAME_OK;
}
// 检查帧是否完整
bool is_frame_complete(const char *frame, uint16_t frame_len,
                       uint16_t *frame_start, uint16_t *frame_end)
{
  for (uint16_t i = 0; i < frame_len; i++)
  {
    if (frame[i] == '#')
    {
      for (uint16_t j = i + 1; j < frame_len; j++)
      {
        if (frame[j] == '$')
        {
          if ((j - i + 1) >= 7)
          { // 最小帧长度
            if (frame_start)
              *frame_start = i;
            if (frame_end)
              *frame_end = j;
            return true;
          }
        }
      }
    }
  }
  return false;
}

// ==================== 消息构建函数 ====================
// 构建查询消息
uint16_t build_query_msg(char *buffer, uint16_t buffer_size, uint8_t seq)
{
  return build_frame(buffer, buffer_size, MSG_QUERY, seq, NULL, 0);
}
// 构建重复查询消息
uint16_t build_repeat_query_msg(char *buffer, uint16_t buffer_size,
                                uint8_t seq)
{
  return build_frame(buffer, buffer_size, MSG_REPEAT_QUERY, seq, NULL, 0);
}
// 构建设置参数消息
uint16_t build_set_params_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                              const param_data_decoded_t *params)
{
  if (!params || buffer_size < 100)
  {
    return 0;
  }

  // 编码参数
  uint8_t raw_data[28]; // 4 * 4 + 2 * 2 + 6 = 28字节
  memset(raw_data, 0, sizeof(raw_data));

  // 编码T1-T4
  raw_data[0] = (params->T1 >> 16) & 0xFF;
  raw_data[1] = (params->T1 >> 8) & 0xFF;
  raw_data[2] = params->T1 & 0xFF;

  raw_data[3] = (params->T2 >> 16) & 0xFF;
  raw_data[4] = (params->T2 >> 8) & 0xFF;
  raw_data[5] = params->T2 & 0xFF;

  raw_data[6] = (params->T3 >> 16) & 0xFF;
  raw_data[7] = (params->T3 >> 8) & 0xFF;
  raw_data[8] = params->T3 & 0xFF;

  raw_data[9] = (params->T4 >> 16) & 0xFF;
  raw_data[10] = (params->T4 >> 8) & 0xFF;
  raw_data[11] = params->T4 & 0xFF;

  // 编码高门限和低门限
  raw_data[12] = (params->threshold_high >> 8) & 0xFF;
  raw_data[13] = params->threshold_high & 0xFF;

  raw_data[14] = (params->threshold_low >> 8) & 0xFF;
  raw_data[15] = params->threshold_low & 0xFF;

  // 编码主设备时间
  memcpy(&raw_data[16], params->master_time, 5);

  char encoded_params[64];
  uint16_t encoded_len = base64_encode(raw_data, 21, encoded_params);

  return build_frame(buffer, buffer_size, MSG_SET_PARAMS, seq, encoded_params,
                     encoded_len);
}
// 构建报告消息
uint16_t build_report_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                          msg_type_t type, const report_data_decoded_t *data)
{
  if (!data || buffer_size < 256)
  {
    return 0;
  }

  // 计算总数据长度
  uint16_t total_data_len = 2 + 2 + data->accel_count * 2; // 温度+湿度+加速度
  uint16_t raw_len =
      5 + 3 + 3 + total_data_len; // tag_id(5) + 时间(3) + 序号(3) + 数据
   uint8_t *raw_data = (uint8_t *)malloc(raw_len);
   if (!raw_data)
     return 0;
//  uint8_t raw_data[512]; // 使用栈上数组替代malloc
  // 编码标签ID
  memcpy(raw_data, data->tag_id, 5);

  // 编码时间
  raw_data[5] = data->start_hour;
  raw_data[6] = data->start_minute;
  raw_data[7] = data->start_second;

  // 编码序号
  raw_data[8] = (data->sequence >> 8) & 0xFF;
  raw_data[9] = data->sequence & 0xFF;

  // 编码温度和湿度
  raw_data[10] = (data->temperature >> 8) & 0xFF;
  raw_data[11] = data->temperature & 0xFF;

  raw_data[12] = (data->humidity >> 8) & 0xFF;
  raw_data[13] = data->humidity & 0xFF;

  // 编码加速度数据
  for (int i = 0; i < data->accel_count; i++)
  {
    raw_data[14 + i * 2] = (data->acceleration[i] >> 8) & 0xFF;
    raw_data[15 + i * 2] = data->acceleration[i] & 0xFF;
  }

  char encoded_data[256];
  uint16_t encoded_len = base64_encode(raw_data, raw_len, encoded_data);

  uint16_t frame_len =
      build_frame(buffer, buffer_size, type, seq, encoded_data, encoded_len);

  free(raw_data);
  return frame_len;
}
// 构建参数确认消息
uint16_t build_ack_msg(char *buffer, uint16_t buffer_size, uint8_t seq,
                       const char *original_content, uint16_t content_len)
{
  return build_frame(buffer, buffer_size, MSG_ACK_PARAMS, seq, original_content,
                     content_len);
}

// ==================== 消息解析函数 ====================
// 解析报告数据
bool parse_report_data(const char *encoded_data, uint16_t data_len,
                       report_data_decoded_t *report)
{
  if (!encoded_data || data_len == 0 || !report)
  {
    return false;
  }

  // 解码数据
  uint8_t decoded_data[256];
  int16_t decoded_len = base64_decode(encoded_data, data_len, decoded_data);
  if (decoded_len < 14)
  { // 最小长度：tag_id(5) + 时间(3) + 序号(2) + 温湿度(4)
    return false;
  }

  // 解析标签ID
  memcpy(report->tag_id, decoded_data, 5);

  // 解析时间
  report->start_hour = decoded_data[5];
  report->start_minute = decoded_data[6];
  report->start_second = decoded_data[7];

  // 解析序号
  report->sequence = (decoded_data[8] << 8) | decoded_data[9];

  // 解析温湿度
  report->temperature = (decoded_data[10] << 8) | decoded_data[11];
  report->humidity = (decoded_data[12] << 8) | decoded_data[13];

  // 解析加速度数据
  uint16_t accel_data_len = decoded_len - 14;
  if (accel_data_len > 0)
  {
    report->accel_count = accel_data_len / 2;
    if (report->acceleration)
    {
      for (int i = 0; i < report->accel_count; i++)
      {
        report->acceleration[i] =
            (decoded_data[14 + i * 2] << 8) | decoded_data[15 + i * 2];
      }
    }
  }
  else
  {
    report->accel_count = 0;
  }

  return true;
}
// 解析参数数据
bool parse_param_data(const char *encoded_data, uint16_t data_len,
                      param_data_decoded_t *params)
{
  if (!encoded_data || data_len != 28 || !params)
  { // 28个Base64字符
    return false;
  }

  uint8_t decoded_data[21];
  int16_t decoded_len = base64_decode(encoded_data, 28, decoded_data);
  if (decoded_len != 21)
  {
    return false;
  }

  // 解析T1-T4
  params->T1 =
      (decoded_data[0] << 16) | (decoded_data[1] << 8) | decoded_data[2];
  params->T2 =
      (decoded_data[3] << 16) | (decoded_data[4] << 8) | decoded_data[5];
  params->T3 =
      (decoded_data[6] << 16) | (decoded_data[7] << 8) | decoded_data[8];
  params->T4 =
      (decoded_data[9] << 16) | (decoded_data[10] << 8) | decoded_data[11];

  // 解析门限
  params->threshold_high = (decoded_data[12] << 8) | decoded_data[13];
  params->threshold_low = (decoded_data[14] << 8) | decoded_data[15];

  // 解析主设备时间
  memcpy(params->master_time, &decoded_data[16], 5);

  return true;
}

// ==================== 数据转换函数 ====================
// 温度转换（摄氏度 <-> 12位编码）
uint16_t temperature_to_12bit(float temp_celsius)
{
  int16_t temp_deci = (int16_t)((temp_celsius + 200.0f) * 10.0f);
  if (temp_deci < 0)
    temp_deci = 0;
  if (temp_deci > 4000)
    temp_deci = 4000;
  return (uint16_t)temp_deci;
}

float temperature_from_12bit(uint16_t data)
{
  return (float)data / 10.0f - 200.0f;
}

uint16_t humidity_to_12bit(float humidity)
{
  uint16_t hum_deci = (uint16_t)(humidity * 10.0f);
  if (hum_deci > 1000)
    hum_deci = 1000;
  return hum_deci;
}

float humidity_from_12bit(uint16_t data) { return (float)data / 10.0f; }

uint16_t acceleration_to_12bit(float accel_g, float full_scale)
{
  float normalized = (accel_g + full_scale) / (2.0f * full_scale) * 4095.0f;
  uint16_t value = (uint16_t)normalized;
  if (value > 4095)
    value = 4095;
  return value;
}

float acceleration_from_12bit(uint16_t data, float full_scale)
{
  return ((float)data / 4095.0f * 2.0f - 1.0f) * full_scale;
}

// ==================== 调试和日志 ====================

void hex_dump(const uint8_t *data, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
  {
    printf("%02X ", data[i]);
    if ((i + 1) % 16 == 0)
      printf("\n");
  }
  printf("\n");
}
// 打印消息帧
void print_frame(const char *frame, uint16_t length)
{
  printf("Frame (%u bytes): ", length);
  for (uint16_t i = 0; i < length; i++)
  {
    if (frame[i] >= 32 && frame[i] <= 126)
    {
      printf("%c", frame[i]);
    }
    else
    {
      printf(".");
    }
  }
  printf("\n");

  printf("Hex: ");
  for (uint16_t i = 0; i < length; i++)
  {
    printf("%02X ", (uint8_t)frame[i]);
  }
  printf("\n");
}
// 打印报告数据
void print_report_data(const report_data_decoded_t *report)
{
  printf("=== Report Data ===\n");
  printf("Tag ID: ");
  for (int i = 0; i < 5; i++)
  {
    printf("%02X", report->tag_id[i]);
  }
  printf("\n");
  printf("Start Time: %02u:%02u:%02u\n", report->start_hour,
         report->start_minute, report->start_second);
  printf("Sequence: %u\n", report->sequence);
  printf("Temperature: %.1f°C\n", temperature_from_12bit(report->temperature));
  printf("Humidity: %.1f%%\n", humidity_from_12bit(report->humidity));
  printf("Acceleration data points: %u\n", report->accel_count);
  if (report->accel_count > 0)
  {
    printf("First 5 acceleration values: ");
    for (int i = 0; i < (report->accel_count < 5 ? report->accel_count : 5);
         i++)
    {
      printf("%.3fg ", acceleration_from_12bit(report->acceleration[i], 16.0f));
    }
    printf("\n");
  }
}
// 打印参数数据
void print_param_data(const param_data_decoded_t *params)
{
  printf("=== Parameter Data ===\n");
  printf("T1: %u ms\n", params->T1);
  printf("T2: %u ms\n", params->T2);
  printf("T3: %u ms\n", params->T3);
  printf("T4: %u ms\n", params->T4);
  printf("Threshold High: %u\n", params->threshold_high);
  printf("Threshold Low: %u\n", params->threshold_low);
  printf("Master Time: ");
  for (int i = 0; i < 5; i++)
  {
    printf("%02X", params->master_time[i]);
  }
  printf("\n");
}