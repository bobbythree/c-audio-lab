#include "wav.h"

// Write a 16-bit unsigned integer in little-endian byte order
bool write_u16(FILE *file, uint16_t value) {
  unsigned char bytes[2] = {(unsigned char)(value & 0xFF),
                            (unsigned char)((value >> 8) & 0xFF)};

  return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}

// Write a 32-bit unsigned integer in little-endian byte order
bool write_u32(FILE *file, uint32_t value) {
  unsigned char bytes[4] = {(unsigned char)(value & 0xFF),
                            (unsigned char)((value >> 8) & 0xFF),
                            (unsigned char)((value >> 16) & 0xFF),
                            (unsigned char)((value >> 24) & 0xFF)};

  return fwrite(bytes, 1, sizeof(bytes), file) == sizeof(bytes);
}
