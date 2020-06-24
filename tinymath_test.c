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
	fixed b = FLOAT_TO_FIXED(f2);

	printf("%s\n", fixed_to_bin_str(a));
	printf("%s\n", fixed_to_bin_str(b));
	printf("%s\n", fixed_to_bin_str(-a));
	printf("%s\n", fixed_to_bin_str(-b));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(add_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(add_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(add_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(add_fixed(-a,-b)));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(sub_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(sub_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(sub_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(sub_fixed(-a,-b)));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(mul_fast_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(mul_fast_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(mul_fast_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(mul_fast_fixed(-a,-b)));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(mul_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(mul_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(mul_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(mul_fixed(-a,-b)));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(div_fast_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(div_fast_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(div_fast_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(div_fast_fixed(-a,-b)));
	printf("\n");
	printf("%s\n", fixed_to_bin_str(div_fixed(a,b)));
	printf("%s\n", fixed_to_bin_str(div_fixed(a,-b)));
	printf("%s\n", fixed_to_bin_str(div_fixed(-a,b)));
	printf("%s\n", fixed_to_bin_str(div_fixed(-a,-b)));

	return 0;
}