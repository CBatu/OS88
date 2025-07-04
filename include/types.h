#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

#define ERR_INDIRECT(errno) (errno##UL)
#define ERR(errno) (-ERR_INDIRECT(errno))

// typedef long ssize_t;
#define INT_MAX 2147483647 // 2^31 - 1

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

#define force_inline inline __attribute__((always_inline))


#include "limine.h"
#include "print.h"
#include "serial/serial.h"

#endif