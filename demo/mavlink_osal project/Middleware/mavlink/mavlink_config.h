// mavlink_config.h
#ifndef MAVLINK_CONFIG_H
#define MAVLINK_CONFIG_H

// 禁用标准库函数，使用HAL库
#define MAVLINK_NO_STDINT_H
#define MAVLINK_NO_CONSOLE
#define MAVLINK_NO_NETWORK
#define MAVLINK_NO_BSD_SOCKETS

// 启用消息统计
#define MAVLINK_STATUSTEXT_PARSE

// 使用内存高效的编码/解码
#define MAVLINK_MEMORY_SAFE

#endif