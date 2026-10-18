#include "pico/audio.h"
#include <pico/audio_i2s.h>
#include <pico/platform/panic.h>
#include <pico/stdlib.h>

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

  // Keep the firmware running.
  for (;;) {
    tight_loop_contents();
  }
}
