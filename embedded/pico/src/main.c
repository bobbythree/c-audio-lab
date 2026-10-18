#include "pico/audio.h"
#include <pico/audio_i2s.h>
#include <pico/stdlib.h>

static audio_format_t audio_format = {
    .format = AUDIO_BUFFER_FORMAT_PCM_S16,
    .sample_freq = 24000,
    .channel_count = 1,
};

static audio_i2s_config_t i2s_config = {
    .data_pin = 9,
    .clock_pin_base = 10,
    .dma_channel = 0,
    .pio_sm = 0,
};

int main(void) {
  const audio_format_t *output_format =
      audio_i2s_setup(&audio_format, &i2s_config);

  if (!output_format) {
    panic("unable to initialize I2S audio");
  }

  for (;;) {
    tight_loop_contents();
  }
}
