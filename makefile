CC = gcc
CFLAGS_PRE = -Wall -std=c99
CFLAGS_POST = -lm

TESTS = $(wildcard test/*.c)
TEST_OUTPUTS = $(patsubst test/%.c,obj/test/%, $(TESTS))

.PHONY: test

test: testbuild testrun

specific:
	$(CC) $(CFLAGS_PRE) -o obj/$(patsubst %.c,%, $(target)) $(target) $(CFLAGS_POST)

testbuild: 
	$(foreach file, $(TESTS), \
		$(CC) $(CFLAGS_PRE) -o obj/$(patsubst test/%.c,test/%, $(file)) $(file) $(CFLAGS_POST); \
		chmod +x obj/$(patsubst test/%.c,test/%, $(file)); )
	
testrun:
	$(foreach file, $(TEST_OUTPUTS), $(file);)