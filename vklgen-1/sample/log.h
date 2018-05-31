#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define LOG(fmt, ...) printf(fmt "\n", __VA_ARGS__)
#define LOG_WARNING LOG
#define LOG_ERROR LOG
#define LOG_DEBUG LOG

#endif //__LOG_H__