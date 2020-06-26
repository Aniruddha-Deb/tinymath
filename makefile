CC = gcc
CFLAGS = -Wall -std=c99 -lm

TESTS = $(wildcard test/*.c)
TEST_OUTPUTS = $(patsubst test/%.c,obj/test/%, $(TESTS))

.PHONY: test

test: 
	$(foreach file, $(TESTS), \
		$(CC) $(CFLAGS) -o obj/$(patsubst test/%.c,test/%, $(file)) $(file); \
		chmod +x obj/$(patsubst test/%.c,test/%, $(file)); )
	$(foreach file, $(TEST_OUTPUTS), $(file);)