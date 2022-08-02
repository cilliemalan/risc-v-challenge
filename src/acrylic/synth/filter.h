#pragma once

typedef struct filter
{
	// coeficients
	float a0, a1, a2;
	float b1, b2;

	// previous values
	float px0, px1, px2;
	float py0, py1, py2;
} filter_t;

void acrylic_filter_lowpass(float frequency, float q, filter_t *filter);
void acrylic_filter_process(float* buffer, unsigned int amt, filter_t *filter);
