#pragma once
#include <stddef.h>
#include "envelope.h"

#define MAX_POLYPHONY 8

typedef enum acrylic_waveform
{
	ACRYLIC_WAVEFORM_SINE,
	ACRYLIC_WAVEFORM_SQUARE,
	ACRYLIC_WAVEFORM_SAW,
	ACRYLIC_WAVEFORM_TRIANGLE,
	ACRYLIC_WAVEFORM_NOISE,
} acrylic_waveform_t;

typedef struct acrylic_oscillator_note
{
	// the progress into the period of oscilation
	float t;
	// note frequency
	float frequency;
	// the velocity of the note when struck
	float velocity;
	// frequency divided by sample rate
	float frequency_by_samplerate;
	// the currently playing note
	char note;
	// the amplitude envelope state
	acrylic_envelope_state_t amplitude_envelope;
	// the filter envelope state
	acrylic_envelope_state_t filter_envelope;
	// the filter envelope state
	acrylic_envelope_state_t pitch_envelope;
} acrylic_oscillator_note_t;

typedef struct acrylic_oscillator
{
	// the waveform of this osciltor
	acrylic_waveform_t waveform;
	// how loud is this oscilator
	float volume;
	// the envelope configuration for amplitude
	acrylic_envelope_t amplitude_envelope;
	// filter cutoff envelope
	acrylic_envelope_t filter_envelope;
	// pitch envelope
	acrylic_envelope_t pitch_envelope;
	// number of (aditional) voices.
	int voices;
	// how much to transpose each note up or down
	// in semitones.
	float detune;

	// VCO depth
	float vco_depth;
	// VCO rate
	float vco_rate;
	// VCO ramp time
	float vco_ramp;
	// VCO waveform
	acrylic_waveform_t vco_waveform;

	// tracking of played notes
	acrylic_oscillator_note_t notes[MAX_POLYPHONY];
} acrylic_oscillator_t;

void acrylic_oscillator_process(float* buffer, unsigned int amt, acrylic_oscillator_t *filter);

