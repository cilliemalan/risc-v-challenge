#include "oscillator.h"
#include "synth.h"
#include "util.h"
#include "assert.h"

#include <math.h>

static float calc_waveform(float t, acrylic_waveform_t type)
{
    switch (type)
    {
    case ACRYLIC_WAVEFORM_SINE:
        //return afsinf((t - 0.5) * 2.0f * PI);
        return sinf((t - 0.5) * 2.0f * PI);
    case ACRYLIC_WAVEFORM_SQUARE:
        return t < 0.5f ? 1.0f : -1.0f;
    case ACRYLIC_WAVEFORM_SAW:
        return (t - 0.5f) * 2.0f;
    case ACRYLIC_WAVEFORM_TRIANGLE:
        if (t < 0.25f)
            return t * 4.0f;
        else if (t < 0.75f)
            return (0.5f - t) * 4.0f;
        else
            return (t - 1.0f) * 4.0f;
    case ACRYLIC_WAVEFORM_NOISE:
        return arandom();
    default:
        return 0;
    }
}

void acrylic_oscillator_process(float *buffer, unsigned int amt, acrylic_oscillator_t *oscillator)
{
    for (unsigned int i = 0; i < amt; i++)
    {
        float total_sample = 0;
        for (int j = 0; j < ARRAYSIZE(oscillator->notes); j++)
        {
            acrylic_oscillator_note_t *note = &oscillator->notes[j];
            if (note->amplitude_envelope.mode != ACRYLIC_OSCILLATOR_MODE_OFF)
            {
                float sample = calc_waveform(note->t, oscillator->waveform);
                float amp_e = acrylic_envelope_tick(&note->amplitude_envelope, &oscillator->amplitude_envelope);

                if (note->amplitude_envelope.mode != ACRYLIC_OSCILLATOR_MODE_OFF)
                {
                    float filt_e = acrylic_envelope_tick(&note->filter_envelope, &oscillator->filter_envelope);
                    float pitch_e = acrylic_envelope_tick(&note->pitch_envelope, &oscillator->pitch_envelope);

                    // turn the 0-1 level into
                    // logarithmic level
                    sample *= level_to_amplitude(amp_e);

                    // adjust note->t based on note frequency
                    float f = note->frequency;

                    // pitch envelope
                    if (oscillator->pitch_envelope_amount != 0.0f)
                    {
                        float octaves = pitch_e * oscillator->pitch_envelope_amount;
                        f *= aexp2(octaves);
                    }

                    // TODO: VCO
                    // TODO: pitch bend

                    // use frequency to adjust t
                    assert(f >= 0.0f);
                    float rate = OO_SAMPLE_RATE * f;
                    float newt = note->t + rate;
                    while (newt > 1.0f)
                    {
                        newt -= 1.0f;
                    }
                    note->t  =newt;

                    total_sample += sample;
                }
                else
                {
                    // the note is done
                    memset(note, 0, sizeof(*note));
                    note->note = 0;
                }
            }
        }

        buffer[i] = total_sample;
    }
}

void acrylic_oscillator_note_trigger(acrylic_oscillator_t *o, char newnote, int velocity)
{
    if (newnote != 0)
    {
        // find a free note
        for (int i = 0; i < ARRAYSIZE(o->notes); i++)
        {
            acrylic_oscillator_note_t *note = &o->notes[i];
            if (note->note == newnote || note->note == 0)
            {
                note->note = newnote;
                note->velocity = velocity;
                note->frequency = calculate_frequency_for_note(newnote);
                acrylic_envelope_trigger(&note->amplitude_envelope, &o->amplitude_envelope);
                acrylic_envelope_trigger(&note->pitch_envelope, &o->pitch_envelope);
                acrylic_envelope_trigger(&note->filter_envelope, &o->filter_envelope);
                break;
            }
        }
    }
}

void acrylic_oscillator_note_release(acrylic_oscillator_t *o, char oldnote)
{
    if (oldnote != 0)
    {
        // find a free note
        for (int i = 0; i < ARRAYSIZE(o->notes); i++)
        {
            acrylic_oscillator_note_t *note = &o->notes[i];
            if (note->note == oldnote)
            {
                acrylic_envelope_release(&note->amplitude_envelope, &o->amplitude_envelope);
                acrylic_envelope_release(&note->pitch_envelope, &o->pitch_envelope);
                acrylic_envelope_release(&note->filter_envelope, &o->filter_envelope);
                break;
            }
        }
    }
}