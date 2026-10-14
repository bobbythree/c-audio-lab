CC = clang
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined -lm

TARGET = csynth

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
