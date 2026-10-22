# rwinit

A Linux-native init manager and service supervisor written in C.

## Project goals

- Correct PID 1 behavior
- Supervised service lifecycle management
- TOML service configuration
- Separate rwctl control utility
- Reliable shutdown and reboot
- Incremental development with automated tests

## Requirements

- GCC or another supported C compiler
- Meson >= 1.2.0
- Ninja
- Python 3 (used by Meson)

## Build

meson setup build
meson compile -C build

## Test

meson test -C build --print-errorlogs

## Run

./build/rwinit --version
./build/rwinit --help

This milestone is only a project skeleton. Do not use it as PID 1.
