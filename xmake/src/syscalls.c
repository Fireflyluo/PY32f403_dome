#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>

// 简单的系统调用实现，避免链接警告
int _close(int file) { return 0; }

int _fstat(int file, struct stat *st) {
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file) { return 1; }

int _lseek(int file, int ptr, int dir) { return 0; }

int _read(int file, char *ptr, int len) { return 0; }

// 实现_write用于printf输出（可以重定向到串口）
int _write(int file, char *ptr, int len) {
  // 这里可以实现串口输出
  // 暂时先简单返回长度
  return len;
}

// 堆管理
extern char _end; // 由链接器定义
static char *heap_end = &_end;

caddr_t _sbrk(int incr) {
  char *prev_heap_end = heap_end;
  // 简单的堆实现
  heap_end += incr;
  return (caddr_t)prev_heap_end;
}