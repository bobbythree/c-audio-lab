#ifndef WAV_H
#define WAV_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

bool write_u16(FILE *file, uint16_t value);
bool write_u32(FILE *file, uint32_t value);

#endif
