CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined -lm
GTKFLAGS = $(shell pkg-config --cflags --libs gtk4)

TARGET = csynth

all: $(TARGET)

$(TARGET): main.c notes.h
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(GTKFLAGS) $(LDFLAGS)

run: $(TARGET)
	GDK_BACKEND=wayland GSK_RENDERER=cairo ./$(TARGET)

wav: examples/wav_generator.c
	$(CC) $(CFLAGS) examples/wav_generator.c -o wav_generator $(LDFLAGS)

clean:
	rm -f $(TARGET) wav_generator

.PHONY: all run clean wav
