#ifndef _TINYMATH_H
#define _TINYMATH_H

#include <inttypes.h>
#include <math.h>
#include <limits.h>

typedef int32_t fixed;

#define FLOAT_TO_FIXED(N) (fixed)(N*(1<<16))

#define add_fixed(a,b) (a+b)
#define sub_fixed(a,b) (a-b)
#define mul_fast_fixed(a,b) ((a>>8)*(b>>8))
#define div_fast_fixed(a,b) (((b>>8) != 0) ? (a<<8)/(b>>8) : \
							(((a<<8) > 0)?INT_MAX:-INT_MAX))

#define FIXED_PI 0x3243F
#define FIXED_E  0x2B7E1

const uint64_t DEC_LOOKUP[] = { 500000000000,
								250000000000,
								125000000000,
								62500000000,
								31250000000,
								15625000000,
								7812500000,
								3906250000,
								1953125000,
								976562500,
								488281250,
								244140625,
								122070312,
								61035156,
								30517578,
								15258789 };

const uint64_t OVEFLOW = 1000000000000L;

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

char get_hex_char(int i) {
	if (i < 10) return i+48;
	else return i+55;
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
		str[33-p] = get_hex_char((f&(1<<i))>>i);		
		i++;
	}
	str[34] = '\0';
	return str;
}

char* fixed_to_hex_str(fixed f) {
	static char str[11];
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
	for (; i<8;p++) {
		if (p == 4) {
			str[9-p] = '.';
			continue;
		}
		str[9-p] = get_hex_char((f & (0xF<<(4*i)))>>(4*i));
		i++;
	}
	str[10] = '\0';
	return str;
}

// Expensive to call!!!
char* fixed_to_dec_str(fixed f) {
	int neg = 0;
	if (f<0) {
		neg = 1;
		f = -f;
	}
	uint16_t intpart = (uint16_t)(f >> 16);
	uint64_t decpart = 0L;
	for (int i=15; i>=0; i--) {
		if ((f & (1 << i))) {
			decpart += DEC_LOOKUP[15-i];
		}
		if (decpart > OVEFLOW) {
			intpart++;
			decpart -= OVEFLOW;
		}
	}
	// trim zeros from decpart
	while (decpart%10 == 0 && decpart > 0) {
		decpart /= 10;
	}
	int len = 3;
	uint16_t intpartcpy = intpart;
	uint64_t decpartcpy = decpart;
	while (intpartcpy > 0) {
		len++;
		intpartcpy /= 10;
	}
	while (decpartcpy > 0) {
		len++;
		decpartcpy /= 10;
	}

	char* str = malloc(len);
	snprintf(str, len, " %d.%ld", intpart, decpart);
	if (neg) str[0] = '-';
	return str;
}
#endif