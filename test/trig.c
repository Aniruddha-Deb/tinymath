#include <stdio.h>
#include "../tinymath.h"

int sin_test() {
	return 0;
}

int cos_test() {
	return 0;
}

int tan_test() {
	return 0;
}

int cosec_test() {
	return 0;
}

int sec_test() {
	return 0;
}

int cot_test() {
	return 0;
}

int main(int argc, char** argv) {	
	int num_tests = 6;
	int (*tests[])(void) = {&sin_test, &cos_test, &tan_test, &cosec_test, &sec_test, &cot_test};
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
