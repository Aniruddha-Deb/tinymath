#ifndef _TINYMATH_H
#define _TINYMATH_H

#include <inttypes.h>

typedef int32_t fixed;

#define FLOAT_TO_FIXED(N) (fixed)(N*(1<<16))

#define add_fixed(a,b) (a+b)
#define sub_fixed(a,b) (a-b)
#define mul_fast_fixed(a,b) ((a>>8)*(b>>8))
#define div_fast_fixed(a,b) ((a<<8)/(b>>8))

fixed mul_fixed(fixed a, fixed b) {
	int sign = 1;
	if (a<0) {
		sign *= -1;
		a = -a;
	}
	if (b<0) {
		sign *= -1;
		b = -b;
	}

	uint64_t along = ((uint64_t)a);
	uint64_t blong = ((uint64_t)b);	
	return sign*((fixed)((along*blong)>>16));
}

fixed div_fixed(fixed a, fixed b) {
	int sign = 1;
	if (a<0) {
		sign *= -1;
		a = -a;
	}
	if (b<0) {
		sign *= -1;
		b = -b;
	}

	uint64_t q = 0;
	uint64_t n = ((uint64_t)a) << 16;
	fixed r = 0;
	for (int i=63; i>=0; i--) {
		r <<= 1;
		r |= ((n & (1<<i))>>i); // set 0th bit of r to ith bit of numerator
		if (r >= b) {
			r -= b;
			q |= (1 << i);
		}
	}
	return sign*((fixed)q);
}

char* fixed_to_bin_str(fixed f) {
	static char str[35];
	if (f<0) {
		str[0]='-';
		f = -f;
	}
	else {
		str[0] = ' ';
	}
	int i,p;
	i=0;
	p=0;
	for (; i<32;p++) {
		if (p == 16) {
			str[33-p] = '.';
			continue;
		}
		// check if ith bit is 1
		if (f & (1<<i)) {
			str[33-p] = '1';
		}
		else {
			str[33-p] = '0';
		}
		i++;
	}
	str[34] = '\0';
	return str;
}
#endif