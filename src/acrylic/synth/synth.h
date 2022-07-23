#pragma once

#define SAMPLE_RATE 48000
#define OO_SAMPLE_RATE 2.083333333333333e-5f // (1.0f / 48000.0f)

#include "envelope.h"
#include "oscillator.h"
#include "filter.h"
#include "util.h"

struct acrylic_synth {
    acrylic_oscillator_t oscillator1;
    acrylic_oscillator_t oscillator2;
} acrylic_synth_t;