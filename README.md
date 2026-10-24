# Audio Synthesis in C

An exploration of audio synthesis and digital signal processing using the C programming language.

This project began as an exercise in generating sound from scratch, with the broader goal of developing a playable synthesizer.

The focus is on learning C, understanding how digital audio works, and gradually experimenting with oscillators, filters, effects, and real-time audio processing.

## Status

Early development. The desktop example generates sine waves and writes 16-bit PCM WAV files without third-party audio libraries. A separate Raspberry Pi Pico project outputs a pulsing 60 Hz square wave over I2S.

## Desktop WAV example

Requires a POSIX environment (such as Linux), CMake 3.20 or newer, a C99 compiler (GCC or Clang), and Make or Ninja. The example uses POSIX file APIs and links the system math library (`libm`).

From the repository root:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/wav_generator
```

The program writes `output.wav` in the current working directory, replacing any existing file of that name after generation succeeds. It contains ten seconds of an 82.41 Hz sine wave at 44.1 kHz, mono, 16-bit PCM.

## Pico I2S firmware

Requires CMake 3.17 or newer, Make or Ninja, the Arm GNU toolchain (`arm-none-eabi-gcc` and `arm-none-eabi-g++`), the Pico SDK with its required submodules, and pico-extras. The local dependency versions used for these instructions are Pico SDK `2.3.1` and pico-extras tag `sdk-2.3.0`.

Set the paths to your SDK checkouts, then configure and build this project separately from the repository root:

```sh
export PICO_SDK_PATH=/path/to/pico-sdk
export PICO_EXTRAS_PATH=/path/to/pico-extras
cmake -S embedded/pico -B embedded/pico/build \
  -DPICO_BOARD=pico -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build embedded/pico/build
```

The SDK may download and build a matching `picotool` if one is not installed. Outputs include `embedded/pico/build/pico_firmware.elf` and `pico_firmware.uf2` in the same directory. Building does not flash the board.

To run on an RP2040 Pico, copy the UF2 to the board's BOOTSEL drive. Audio requires a compatible I2S DAC or Audio Pack connected to GPIO 9 (data), GPIO 10 (BCLK), and GPIO 11 (LRCLK), with suitable power and common ground. The firmware produces 24 kHz, 16-bit mono samples for I2S playback; it is not a desktop executable.

## Neovim and clangd

The desktop CMake project exports `build/compile_commands.json`; the Pico configure command above enables its separate database. Keep the databases separate so clangd uses the correct compiler and SDK settings.

For a fresh checkout, after configuring both projects, create these links from the repository root if they do not already exist:

```sh
ln -s build/compile_commands.json compile_commands.json
ln -s build/compile_commands.json embedded/pico/compile_commands.json
```

Use Neovim with clangd enabled. These local links and the generated databases are ignored by Git.

