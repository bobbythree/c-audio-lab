CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined -lm
SDLFLAGS = $(shell pkg-config --cflags --libs sdl3)

TARGET = csynth

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LDFLAGS) $(SDLFLAGS)

wav: examples/wav_generator.c
	$(CC) $(CFLAGS) examples/wav_generator.c -o wav_generator $(LDFLAGS)

clean:
	rm -f $(TARGET) wav_generator

.PHONY: all clean wav
