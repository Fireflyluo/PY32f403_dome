// main.c - 简单的协议测试程序
#include "data_protocol.h"
#include <math.h>
#include <stdio.h>

#include <string.h>

// 打印十六进制
void print_hex(const char *label, const uint8_t *data, uint16_t len) {
  printf("%s: ", label);
  for (uint16_t i = 0; i < len; i++) {
    printf("%02X ", data[i]);
  }
  printf("\n");
}

int main(void) {
  printf("=== 数据传输协议测试程序 ===\n\n");

  // 测试1: Base64编解码
  printf("1. Base64编解码测试:\n");
  {
    uint8_t test_data[] = {0x12, 0x13, 0x14, 0x16, 0x18, 0x20};
    char encoded[32];
    uint8_t decoded[16];

    printf("   原始数据: ");
    for (int i = 0; i < 6; i++)
      printf("%02X ", test_data[i]);
    printf("\n");

    uint16_t enc_len = base64_encode(test_data, 6, encoded);
    printf("   编码后: %s (长度: %u)\n", encoded, enc_len);

    int16_t dec_len = base64_decode(encoded, enc_len, decoded);
    printf("   解码后: ");
    for (int i = 0; i < dec_len; i++)
      printf("%02X ", decoded[i]);
    printf("(长度: %d)\n", dec_len);

    if (dec_len == 6 && memcmp(test_data, decoded, 6) == 0) {
      printf("   ? Base64测试通过\n");
    } else {
      printf("   ? Base64测试失败\n");
    }
  }

  // 测试2: 消息构建和解析
  printf("\n2. 消息构建与解析测试:\n");
  {
    char frame[256];

    // 构建查询消息
    uint16_t len = build_query_msg(frame, sizeof(frame), 5);
    printf("   构建的查询消息: ");
    for (uint16_t i = 0; i < len; i++)
      printf("%c", frame[i]);
    printf(" (长度: %u)\n", len);

    // 解析消息
    msg_type_t type;
    uint8_t seq;
    char content[256];
    uint16_t content_len;
    uint32_t crc;

    frame_status_t status =
        parse_frame(frame, len, &type, &seq, content, &content_len, &crc);

    if (status == FRAME_OK) {
      printf("   解析成功: 类型='%c', 序列号=%u, CRC=0x%06X\n", (char)type, seq,
             crc);
      printf("   ? 消息解析测试通过\n");
    } else {
      printf("   解析失败: 错误码=%d\n", status);
      printf("   ? 消息解析测试失败\n");
    }
  }

  // 测试3: CRC计算
  printf("\n3. CRC24Q计算测试:\n");
  {
    uint8_t test_data[] = "123456789";
    uint32_t crc = crc24q_calculate(test_data, 9);
    printf("   CRC24Q('123456789') = 0x%06X\n", crc);

    char crc_str[5];
    crc_to_base64(crc, crc_str);
    printf("   CRC Base64: %s\n", crc_str);

    uint32_t crc_back = crc_from_base64(crc_str);
    if (crc == crc_back) {
      printf("   ? CRC测试通过\n");
    } else {
      printf("   ? CRC测试失败\n");
    }
  }

  // 测试4: 数据转换
  printf("\n4. 数据转换测试:\n");
  {
    // 温度转换
    float temp = 25.5f;
    uint16_t temp_enc = temperature_to_12bit(temp);
    float temp_dec = temperature_from_12bit(temp_enc);
    printf("   温度转换: %.1f°C -> 0x%03X -> %.1f°C ", temp, temp_enc,
           temp_dec);
    if (fabsf(temp - temp_dec) < 0.1f) {
      printf("?\n");
    } else {
      printf("?\n");
    }

    // 湿度转换
    float humidity = 65.5f;
    uint16_t hum_enc = humidity_to_12bit(humidity);
    float hum_dec = humidity_from_12bit(hum_enc);
    printf("   湿度转换: %.1f%% -> 0x%03X -> %.1f%% ", humidity, hum_enc,
           hum_dec);
    if (fabsf(humidity - hum_dec) < 0.1f) {
      printf("?\n");
    } else {
      printf("?\n");
    }
  }

  // 测试5: 完整工作流程
  printf("\n5. 完整工作流程测试:\n");
  {
    printf("   模拟主机发送查询消息...\n");
    char tx_frame[256];
    uint16_t tx_len = build_query_msg(tx_frame, sizeof(tx_frame), 1);
    printf("   发送: ");
    for (uint16_t i = 0; i < tx_len; i++)
      printf("%c", tx_frame[i]);
    printf("\n");

    printf("   模拟设备接收并响应...\n");
    char rx_frame[256];

    uint16_t accel_data[3] = {2048, 2049, 2050};
    report_data_decoded_t report = {.tag_id = {0x01, 0x02, 0x03, 0x04, 0x05},
                                    .start_hour = 10,
                                    .start_minute = 30,
                                    .start_second = 45,
                                    .sequence = 1,
                                    .temperature = temperature_to_12bit(25.5f),
                                    .humidity = humidity_to_12bit(65.0f),
                                    .acceleration = accel_data,
                                    .accel_count = 3};

    // 构建一个实际的报告消息
    uint16_t rx_len = build_report_msg(rx_frame, sizeof(rx_frame), 1,
                                       MSG_REPORT_FIRST, &report);

    if (rx_len > 0) {
      printf("   响应: ");
      for (uint16_t i = 0; i < rx_len; i++)
        printf("%c", rx_frame[i]);
      printf("\n");

      // 解析响应消息
      msg_type_t rx_type;
      uint8_t rx_seq;
      char rx_content[256];
      uint16_t rx_content_len;
      uint32_t rx_crc;

      frame_status_t rx_status =
          parse_frame(rx_frame, rx_len, &rx_type, &rx_seq, rx_content,
                      &rx_content_len, &rx_crc);

      if (rx_status == FRAME_OK) {
        printf("   主机解析响应: 类型='%c', 序列号=%u\n", (char)rx_type,
               rx_seq);

        if (rx_type == MSG_REPORT_FIRST) {
          report_data_decoded_t parsed_report;
          if (parse_report_data(rx_content, rx_content_len, &parsed_report)) {
            printf("   解析报告数据成功\n");
            printf("   标签ID: ");
            for (int i = 0; i < 5; i++)
              printf("%02X", parsed_report.tag_id[i]);
            printf("\n");
            printf("   温度: %.1f°C\n",
                   temperature_from_12bit(parsed_report.temperature));
          }
        }
      }

      printf("   ? 工作流程测试完成\n");
    } else {
      printf("   [ERROR] 无法构建响应消息\n");
    }
  }

  printf("\n=== 测试完成 ===\n");
  return 0;
}