CC = gcc
CFLAGS = -Wall -std=c99 -lm

tinymath_test: tinymath_test.c tinymath.h
	$(CC) $(CFLAGS) -o obj/tmath_test tinymath_test.c
