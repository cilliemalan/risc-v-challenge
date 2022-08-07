#include "util.h"
#include <stdint.h>
#include <math.h>

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


float aexp2(float input)
{
	// TODO: ...find a fast thing I guess
	return exp2f(input);
}

// the famous quake inverse square root
float ainvsqrt(float input)
{
	union
	{
		float single;
		uint_least32_t uint;
	} u;

	// fast inverse square root x^-0.5
	u.single = input;
	u.uint = (0xbe6eb3beU - u.uint) >> 1;
	return u.single;
}

float areciprocal(float input)
{
	// square it to get x^-1
	float z = ainvsqrt(input);
	return z * z;
}

float alog10(float input)
{
	// this is not a very close aproximation
	// but it intersects at 1 and is asimptotic to 0
	// so it will work well enough I think.
	// this is only for values between 0 and 1
	//(2−2/𝑥^0.5 )/ln(10)
	static const float ooln10times2 = 0.868588963f;
	float isr = ooln10times2 - ooln10times2 * ainvsqrt(input);
	return isr;
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
	return alerp(a, b, t);
}

float aelerp(float a, float b, float t)
{
	return aglerp(a, b, t, -4);
}

// can't remember where I got these...
float afsinf(float x)
{
	if (x < 0)
		return 1.27323954f * x + .405284735f * x * x;
	else
		return 1.27323954f * x - 0.405284735f * x * x;
}

float afcosf(float x)
{
	x += 1.57079632f;
	if (x > 3.14159265f)
		x -= 6.28318531f;

	if (x < 0)
		return 1.27323954f * x + 0.405284735f * x * x;
	else
		return 1.27323954f * x - 0.405284735f * x * x;
}

float arandom()
{
	// from http://burtleburtle.net/bob/hash/integer.html
	static unsigned int seed = 0;
	unsigned int a = ++seed;
	a = (a ^ 61) ^ (a >> 16);
	a = a + (a << 3);
	a = a ^ (a >> 4);
	a = a * 0x27d4eb2d;
	a = a ^ (a >> 15);

	static const float factor = 1.0f / 0x7fffffff;
	return (int)a * factor;
}

float calculate_frequency_for_note(char note)
{
	// 2^(N/12)
	static const float notes[12] = {
		1.0f,
		1.059463094f,
		1.122462048f,
		1.189207115f,
		1.25992105f,
		1.334839854f,
		1.414213562f,
		1.498307077f,
		1.587401052f,
		1.681792831f,
		1.781797436f,
		1.887748625f,
	};

	// 2^(n - 5)
	static const float octaves[11] = {
		0.03125f,
		0.0625f,
		0.125f,
		0.25f,
		0.5f,
		1.0f,
		2.0f,
		4.0f,
		8.0f,
		16.0f,
		32.0f,
	};

	// 0 -> C in octave -1
	static const float a3 = 440.0f;
	// static const float c3 = (float)(a3 * notes[3]);
	static const float c3 = 440.0f * 1.189207115f;

	// reshift octaves back to 0
	int n = note - 12;

	// freq is base * 2^(n/12) where base is based on note and octave
	uint8_t octave = n / 12;
	uint8_t note_on_octave = n % 12;

	// octave's C frequency is 2^(octave) * c4
	// note is (octave C frequency) * 2^(note/12)

	float oct_f = octave < 11 ? octaves[octave] : 0;
	float note_f = notes[note_on_octave];

	return oct_f * note_f * c3;
}
