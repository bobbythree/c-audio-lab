#include "pico/audio.h"
#include <pico/audio_i2s.h>
#include <pico/platform/panic.h>
#include <pico/stdlib.h>
#include <stdint.h>

// Define 24 kHz, 16-bit mono PCM audio.
static audio_format_t audio_format = {
    .format = AUDIO_BUFFER_FORMAT_PCM_S16,
    .sample_freq = 24000,
    .channel_count = 1,
};

// Define how samples are stored in memory.
static audio_buffer_format_t buffer_format = {
    .format = &audio_format,
    .sample_stride = sizeof(int16_t),
};

// Configure I2S pins and hardware resources for the Audio Pack.
static audio_i2s_config_t i2s_config = {
    .data_pin = 9,
    .clock_pin_base = 10,
    .dma_channel = 0,
    .pio_sm = 0,
};

int main(void) {
  // Initialize I2S output using our audio format and hardware settings.
  const audio_format_t *output_format =
      audio_i2s_setup(&audio_format, &i2s_config);

  if (!output_format) {
    panic("unable to initialize I2S audio");
  }

  // Allocate three buffers holding 256 PCM samples each.
  audio_buffer_pool_t *producer_pool =
      audio_new_producer_pool(&buffer_format, 3, 256);

  if (!producer_pool) {
    panic("unable to create audio buffer pool");
  }

  // Connect the sample producer to the I2S audio driver.
  if (!audio_i2s_connect(producer_pool)) {
    panic("unable to connect audio buffer pool");
  }

  // start I2S audio output
  audio_i2s_set_enabled(true);

  uint32_t sample_position = 0;
  uint32_t pulse_position = 0;

  // Keep the firmware running.
  for (;;) {
    // Wait for an available audio buffer.
    audio_buffer_t *buffer = take_audio_buffer(producer_pool, true);

    // Access the buffer as signed 16-bit PCM samples.
    int16_t *samples = (int16_t *)buffer->buffer->bytes;

    // Generate a 60 Hz square wave with slow amplitude pulses.
    for (uint32_t i = 0; i < buffer->max_sample_count; i++) {
      int16_t amplitude = (pulse_position < 1200) ? 300 : 100;
      samples[i] = (sample_position < 200) ? amplitude : -amplitude;
      sample_position = (sample_position + 1) % 400;
      pulse_position = (pulse_position + 1) % 24000;
    }

    // Submit the filled buffer for playback.
    buffer->sample_count = buffer->max_sample_count;
    give_audio_buffer(producer_pool, buffer);
  }
}
