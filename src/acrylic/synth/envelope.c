#include "envelope.h"
#include "synth.h"

void acrylic_envelope_initialize(acrylic_envelope_t *envelope)
{
    // OO_SAMPLE_RATE is the minimum attack/decay/release time
    // and results in the maximum rate of 1

    if (envelope->attack > OO_SAMPLE_RATE)
        envelope->_attack_rate = OO_SAMPLE_RATE / envelope->attack;
    else
        envelope->_attack_rate = 1;

    if (envelope->decay > OO_SAMPLE_RATE)
        envelope->_decay_rate = OO_SAMPLE_RATE / envelope->decay;
    else
        envelope->_decay_rate = 1;

    if (envelope->release > OO_SAMPLE_RATE)
        envelope->_release_rate = OO_SAMPLE_RATE / envelope->release;
    else
        envelope->_release_rate = 1;
}

float acrylic_envelope_tick(acrylic_envelope_state_t *state, acrylic_envelope_t *envelope)
{
    switch (state->mode)
    {
    case ACRYLIC_OSCILLATOR_MODE_ATTACK:
        state->level += envelope->_attack_rate;
        if (state->level >= 1.0f)
        {
            state->level = 1.0f;
            state->mode = ACRYLIC_OSCILLATOR_MODE_DECAY;
        }
        break;
    case ACRYLIC_OSCILLATOR_MODE_DECAY:
        state->level -= envelope->_decay_rate;
        if (state->level <= envelope->sustain)
        {
            state->level = envelope->sustain;
            state->mode = ACRYLIC_OSCILLATOR_MODE_SUSTAIN;
        }
        break;
    case ACRYLIC_OSCILLATOR_MODE_SUSTAIN:
        break;
    case ACRYLIC_OSCILLATOR_MODE_RELEASE:
        state->level -= state->level * envelope->_release_rate;
        if (state->level <= 0.0f)
        {
            state->level = 0.0f;
            state->mode = ACRYLIC_OSCILLATOR_MODE_OFF;
        }
        break;
    case ACRYLIC_OSCILLATOR_MODE_OFF:
        break;
    }

    return state->level;
}

void acrylic_envelope_trigger(acrylic_envelope_state_t *state, acrylic_envelope_t *envelope)
{
    state->mode = ACRYLIC_OSCILLATOR_MODE_ATTACK;
}

void acrylic_envelope_release(acrylic_envelope_state_t *state, acrylic_envelope_t *envelope)
{
    state->mode = ACRYLIC_OSCILLATOR_MODE_RELEASE;
}
