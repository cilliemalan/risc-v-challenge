#include "synth.h"
#include "oscillator.h"
#include "util.h"

void acrylic_synth_process(acrylic_synth_t *a, float *data, int num_samples)
{
    acrylic_oscillator_process(data, num_samples, &a->oscillator1);
}

void acrylic_synth_note_trigger(acrylic_synth_t *a, char note, int velocity)
{
    acrylic_oscillator_note_trigger(&a->oscillator1, note, velocity);
}

void acrylic_synth_note_release(acrylic_synth_t *a, char note)
{
    acrylic_oscillator_note_release(&a->oscillator1, note);
}
