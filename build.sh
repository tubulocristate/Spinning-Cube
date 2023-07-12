#!/bin/sh

set -xe
clang -Wall -Wextra -o main main.c -lm -lSDL2;
