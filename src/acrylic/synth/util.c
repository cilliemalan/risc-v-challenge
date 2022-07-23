#include "util.h"
    #include <stdint.h>

// Fast power-of-10 approximation, with RMS error of 1.77%.
// This approximation developed by Nicol Schraudolph (Neural Computation vol 11, 1999).
// Adapted for 32-bit floats by Lippold Haken of Haken Audio, April 2010.
// Set float variable's bits to integer expression.
float apow10(float input)
{
	float _f = input;
	float *f = &_f;
	*(int *)f = *f * 27866352.6f + 1064866808.0f;
	return _f;
}

// from https://stackoverflow.com/a/39714493
float areciprocal(float input)
{
	union
	{
		float single;
		uint_least32_t uint;
	} u;

	// fast inverse square root x^-0.5
	u.single = input;
	u.uint = (0xbe6eb3beU - u.uint) >> 1;
	
	// then square it to get x^-1
	u.single *= u.single;
	return u.single;
}

float alerp(float a, float b, float t)
{
	return b * t + (1.0f - t) * a;
}

float aglerp(float a, float b, float t, float g)
{
	if (g != 1.0f)
	{
		t = (exp2f(g * t) - 1) / (exp2f(g) - 1);
	}
	return lerp(a, b, t);
}

float aelerp(float a, float b, float t)
{
	return glerp(a, b, t, -4);
}
