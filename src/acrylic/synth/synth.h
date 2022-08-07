#pragma once

#define SAMPLE_RATE 48000
#define OO_SAMPLE_RATE 2.083333333333333e-5f // (1.0f / 48000.0f)

#include "envelope.h"
#include "oscillator.h"
#include "filter.h"
#include "util.h"
#include "limiter.h"

typedef struct acrylic_synth {
    acrylic_oscillator_t oscillator;
    acrylic_limiter_t limiter;
} acrylic_synth_t;

void acrylic_synth_init(acrylic_synth_t *s);
void acrylic_synth_process(acrylic_synth_t *a, float *data, int num_samples);
void acrylic_synth_note_trigger(acrylic_synth_t *a, char note, int velocity);
void acrylic_synth_note_release(acrylic_synth_t *a, char note);

