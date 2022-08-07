#include "acrylic.h"
#include "synth/synth.h"
#include <memory.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

struct acrylic
{
    acrylic_config_t config;
    acrylic_synth_t synth;
};

static void init_synth(acrylic_synth_t* s)
{
    s->oscillator1.amplitude_envelope.attack = 0;
    s->oscillator1.amplitude_envelope.decay = 0;
    s->oscillator1.amplitude_envelope.sustain = 1;
    s->oscillator1.amplitude_envelope.release = 0;
    s->oscillator1.volume = 1;
    s->oscillator1.waveform = ACRYLIC_WAVEFORM_SAW;
    acrylic_envelope_initialize(&s->oscillator1.amplitude_envelope);
}

acrylic_t acrylic_new(acrylic_config_t *config)
{
    acrylic_t a = (acrylic_t)calloc(1, sizeof(struct acrylic));
    memcpy(&a->config, config, sizeof(acrylic_config_t));

    init_synth(&a->synth);
    return a;
}

void acrylic_event(acrylic_t a, acrylic_event_type_t type, acrylic_event_t *event)
{
    switch (type)
    {
    case ACRYLIC_EVENT_TYPE_BUTTON_DOWN:
        if (event->button.number <= 16)
        {
            char note = 57 + event->button.number;
            acrylic_synth_note_trigger(&a->synth, note, 100);
        }
        break;
    case ACRYLIC_EVENT_TYPE_BUTTON_UP:
        if (event->button.number <= 16)
        {
            char note = 57 + event->button.number;
            acrylic_synth_note_release(&a->synth, note);
        }
        break;
    case ACRYLIC_EVENT_TYPE_ENCODER:
        break;
    case ACRYLIC_EVENT_TYPE_MIDI:
        break;
    }
}

void acrylic_process(acrylic_t a, float *data, int num_samples)
{
    acrylic_synth_process(&a->synth, data, num_samples);
}

void acrylic_free(acrylic_t instance)
{
    free(instance);
}
