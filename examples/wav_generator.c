#define _POSIX_C_SOURCE 200809L

#include "../audio/wav.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Audio settings
#define SAMPLE_RATE 44100
#define PI 3.14159265358979323846
#define DURATION 10
#define WAV_HEADER_SIZE 44

int main(void) {
  // Set tone frequency
  double frequency = 82.41;

  // Calculate sample count and WAV file sizes
  uint64_t num_samples = (uint64_t)SAMPLE_RATE * DURATION;
  uint64_t data_size = num_samples * sizeof(int16_t);

  if (data_size > UINT32_MAX - (WAV_HEADER_SIZE - 8)) {
    fprintf(stderr, "Error: audio duration exceeds WAV size limit\n");
    return 1;
  }

  uint32_t wav_data_size = (uint32_t)data_size;
  uint32_t file_size = (WAV_HEADER_SIZE - 8) + wav_data_size;

  // Create a binary WAV file
  const char *output_path = "output.wav";
  char temp_path[] = "output.wav.tmp.XXXXXX";

  int fd = mkstemp(temp_path);
  if (fd == -1) {
    perror("failed to create temporary file");
    return 1;
  }

  FILE *file = fdopen(fd, "wb");
  if (file == NULL) {
    perror("failed to create file stream");

    if (close(fd) == -1) {
      perror("failed to close file descriptor");
    }

    if (unlink(temp_path) == -1) {
      perror("failed to remove temporary file");
    }

    return 1;
  }

  // Write the 44-byte WAV header
  if (fwrite("RIFF", 1, 4, file) != 4 || !write_u32(file, file_size) ||
      fwrite("WAVE", 1, 4, file) != 4 || fwrite("fmt ", 1, 4, file) != 4 ||
      !write_u32(file, 16) || !write_u16(file, 1) || !write_u16(file, 1) ||
      !write_u32(file, SAMPLE_RATE) || !write_u32(file, SAMPLE_RATE * 2) ||
      !write_u16(file, 2) || !write_u16(file, 16) ||
      fwrite("data", 1, 4, file) != 4 || !write_u32(file, wav_data_size)) {
    perror("failed to write WAV header");
    goto cleanup;
  }

  // Generate one PCM sample per iteration
  for (uint64_t i = 0; i < num_samples; i++) {
    // Calculate time in seconds
    double time = (double)i / SAMPLE_RATE;

    // Generate sine wave amplitude between -1 and 1
    double sample = sin(2.0 * PI * frequency * time);

    // Convert amplitude to signed 16-bit PCM
    int16_t pcm = (int16_t)(sample * 32767);

    // Write the sample and check for errors
    if (!write_u16(file, (uint16_t)pcm)) {
      perror("failed to write sample");
      goto cleanup;
    }
  }

  // Close the file and check for errors
  if (fclose(file) == EOF) {
    perror("failed to close WAV file");
    goto remove_temp;
  }

  // Rename completed WAV file
  if (rename(temp_path, output_path) != 0) {
    perror("failed to rename WAV file");
    goto remove_temp;
  }

  return 0;

cleanup:
  if (fclose(file) == EOF) {
    perror("failed to close WAV file");
  }

remove_temp:
  if (remove(temp_path) != 0) {
    perror("failed to remove temporary WAV file");
  }

  return 1;
}
