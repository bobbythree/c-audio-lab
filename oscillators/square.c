#include <stdio.h>

#define SAMPLE_RATE 44100

int main(void) {
  double freq = 440;
  double samples_per_cycle = (double)SAMPLE_RATE / freq;

  printf("Samples per cylce: %.2f\n", samples_per_cycle);

  return 0;
}
