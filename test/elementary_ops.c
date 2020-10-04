#include <stdio.h>
#include "../tinymath.h"
#include <math.h>

#define RANGE_MIN -5.0f
#define RANGE_MAX 5.0f
#define DELTA 0.1f
#define ERROR 0.001f

int addition_test() {
	float f1 = RANGE_MIN;
	float f2 = RANGE_MIN;
	while (f1 < RANGE_MAX) {
		while (f2 < RANGE_MAX) {
			fixed fix1 = FLOAT_TO_FIX(f1);
			fixed fix2 = FLOAT_TO_FIX(f2);
			fixed sum = add_fixed(fix1, fix2);
			if (sum - FLOAT_TO_FIX(f1+f2) > mul_fixed(sum, ERROR)) {
				printf("addition failed %f+%f\n", f1, f2);
				goto fail;
			}
			f2 += DELTA;
		}
		f1 += DELTA;
	}
	return 0;

	fail: 
		return -1;
}

// TODO figure out how to do this and division test
int multiplication_test() {
	return 0;
}

int division_test() {
	return 0;
}

int main(int argc, char** argv) {
	
	int num_tests = 3;
	int (*tests[])(void) = { &addition_test, &multiplication_test, &division_test };
	int results[num_tests];

	for (int i=0; i<num_tests; i++) {
		results[i] = tests[i]();
		if (results[i] < 0) {
			printf("%d test failed\n", i);
			break;
		}
		else {
			printf("%d test passed\n", i);
		}
	}
	return 0;
}
