#ifndef _TINYMATH_H
#define _TINYMATH_H

#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

typedef int32_t fixed;

#define FLOAT_TO_FIXED(N) (fixed)(N*(1<<16))
#define INT_TO_FIXED(N)   (fixed)(N<<16)
#define FIXED_TO_FLOAT(N) (((float)N)/(1<<16))

#define add_fixed(a,b) (a+b)
#define sub_fixed(a,b) (a-b)
#define mul_fast_fixed(a,b) ((a>>8)*(b>>8))
#define div_fast_fixed(a,b) (((b>>8) != 0) ? (a<<8)/(b>>8) : \
							(((a<<8) > 0)?INT_MAX:-INT_MAX))

#define FIXED_PI 0x3243F
#define FIXED_E  0x2B7E1
#define FIXED_SQRT2 0x16A0A

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

char* fixed_to_bin_str(fixed);
char* fixed_to_dec_str(fixed);
void print_fixed(char*, fixed);

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

int get_scale(fixed f) {
	int scale = 15;
	while (scale > -16) {
		if (f&(1<<(scale+15))) return scale;
		scale--;
	}
	return scale;
}

// Works only for 1<f<2
fixed sqrt_NR_fixed(fixed f, int niter) {
	fixed rf = (1<<16) + ((f-(1<<16))>>1);
	for (int i=0; i<niter; i++) {
		rf = mul_fixed(1<<15,rf+div_fixed(f,rf));
	}
	return rf;
}

// Works only for 1<f<2
// very poor accuracy though; NR is better.
fixed sqrt_taylor_fixed(fixed f) {
	f -= (1<<16);
	fixed f2 = mul_fixed(f, f);
	fixed f3 = mul_fixed(f2, f);
	fixed rf = (1<<16) + (f>>1) - (f2>>3) + (f3>>4);
	return rf;
}

fixed sqrt_fast_fixed(fixed f) {
	if (f == 0) return 0;
	else if (f < 0) return -INT_MAX;
	else if (f == (1<<16)) return (1<<16);
	else {
		// sqrt(f) = sqrt(2^div) * sqrt(f/2^div);
		// 1 < f/2^div < 2 . This allows for more precise NR estimation.
		// Also allows creation of a lookup table for sqrt(x), 1<x<2 which further
		// reduces time at the cost of space.
		// 2^div is the scaling factor here
		int div = get_scale(f)-1;
		f = div_fixed(f,(1<<(div+16)));
		fixed root = (1<<16);
		if (div % 2 != 0) {
			// odd power of 2; use sqrt(2)
			root = FIXED_SQRT2;
		}
		// in C, 3/2 = 1 but (-3)/2 = -1 and not -2
		if (div < 0) div--;
		fixed sf_root = 1<<(16+div/2);
		root = mul_fixed(root, sf_root);
		root = mul_fixed(root, sqrt_NR_fixed(f, 2)); // NR for 2 iterations
		return root;
	}
}

fixed pow_fixed_to_int(fixed f, int p) {
	if (p == 0) return (1<<16);
	else if (p == 1) return f;
	else if (p == 2) return mul_fixed(f,f);
	if (p%2 != 0) {
		fixed y = mul_fixed(f, pow_fixed_to_int(f,p-1));
		return y;
	}
	else {
		fixed y = pow_fixed_to_int(f,p/2);
		return mul_fixed(y,y);
	}
}

fixed pow_fixed_to_frac(fixed f, fixed p) {
	fixed pow = (1<<16);
	for (int i=15; i>=0; i--) {
		if (p&(1<<i)) {
			fixed temp = sqrt_fast_fixed(f);
			for (int j=0; j<(15-i); j++) {
				temp = sqrt_fast_fixed(temp);
			}
			pow = mul_fixed(pow,temp);
		}
	}
	return pow;
}

fixed pow_fixed(fixed f, fixed p) {
	int inv = 0;
	if (p<0) {
		inv = 1;
		p = -p;
	}
	int p_int = (p>>16);
	int p_frac = (p & ((1<<16)-1));
	fixed pow = mul_fixed(
						pow_fixed_to_int(f,p_int),
						pow_fixed_to_frac(f,p_frac)
				);
	if (inv) {
		return div_fixed((1<<16), pow);
	}
	return pow;
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
// TODO refactor this hacky crap
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
	int len = 3 + 12;
	uint16_t intpartcpy = intpart;
	while (intpartcpy > 0) {
		len++;
		intpartcpy /= 10;
	}

	char* str = malloc(len);
	snprintf(str, len, " %d.%012ld", intpart, decpart);
	// trim trailing zeros
	for (int i=len-2; i>=0; i--) {
		if (str[i] == '0') len--;
		else break;
	}
	str[len-1] = '\0';
	if (neg) str[0] = '-';
	return str;
}

void print_fixed(char* prefix, fixed f) {
	char* f_str = fixed_to_dec_str(f);
	printf("%s%s\n", prefix, f_str);
	free(f_str);
}

void print_fixed_hex(char* prefix, fixed f) {
	printf("%s%s\n", prefix, fixed_to_hex_str(f));
}
#endif