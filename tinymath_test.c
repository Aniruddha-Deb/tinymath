#include <stdio.h>
#include <stdlib.h>
#include "tinymath.h"

int main(int argc, char** argv) {
	float f1 = 6;
	float f2 = 4;
	if (argc > 1) {
		f1 = atof(argv[1]);
	}
	if (argc > 2) {
		f2 = atof(argv[2]);
	}

	fixed a = FLOAT_TO_FIXED(f1);
	print_fixed("a = ", a);
	fixed b = FLOAT_TO_FIXED(f2);
	print_fixed("b = ", b);

	print_fixed("sqrt(a) = ", sqrt_fast_fixed(a));
	print_fixed("pow(a,b) = ", pow_fixed(a,b));
	return 0;
}