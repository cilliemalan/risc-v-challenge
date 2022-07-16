#include "util.h"

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

float adecay(float a, float b, float rate)
{
	float diff = b - a;
	if (diff < 1.0e-6f)
	{
		return b;
	}
	else
	{
		return a + diff * rate;
	}
}

float aattack(float a, float d, float s, float t)
{
	return lerp(0, 1, t / a);
}

float arelease(float t, float s, float r)
{
	if (r <= 1.0e-6 || t >= r)
	{
		return 0;
	}
	else
	{
		return elerp(s, 0, t / r);
	}
}
