#include <stdio.h>
#include "../tinymath.h"

int sqrt_test() {
	return 0;
}

int pow_test() {
	return 0;
}	

int main(int argc, char** argv) {	
	int num_tests = 0;
	int (*tests[])(void) = {&sqrt_test, &pow_test};
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
