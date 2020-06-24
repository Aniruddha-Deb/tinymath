#include <stdio.h>
#include <math.h>

int main() {
	for (int i=1; i<=16; i++) {
		printf("2^(-%d) = %.12lf\n", i, pow(2,-i));
	}
}