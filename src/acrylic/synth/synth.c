#include "synth.h"
#include "oscillator.h"
#include "util.h"

#include <windows.h>
#include <SetupAPI.h>

void acrylic_synth_init(acrylic_synth_t *s)
{
    s->oscillator.amplitude_envelope.attack = 0;
    s->oscillator.amplitude_envelope.decay = 0;
    s->oscillator.amplitude_envelope.sustain = 1;
    s->oscillator.amplitude_envelope.release = 0;
    s->oscillator.volume = 1;
    s->oscillator.waveform = ACRYLIC_WAVEFORM_PULSE;
    acrylic_envelope_initialize(&s->oscillator.amplitude_envelope);
    acrylic_limiter_init(&s->limiter, 0.7f);
}

void acrylic_synth_process(acrylic_synth_t *a, float *data, int num_samples)
{
    acrylic_oscillator_process(data, num_samples, &a->oscillator);
    acrylic_limiter_process(&a->limiter, data, num_samples);
}

void acrylic_synth_note_trigger(acrylic_synth_t *a, char note, int velocity)
{
    acrylic_oscillator_note_trigger(&a->oscillator, note, velocity);
}

void acrylic_synth_note_release(acrylic_synth_t *a, char note)
{
    acrylic_oscillator_note_release(&a->oscillator, note);
}
