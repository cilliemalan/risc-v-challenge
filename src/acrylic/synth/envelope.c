#include "envelope.h"
#include "synth.h"

void acrylic_envelope_initialize(acrylic_envelope_t *envelope)
{
	envelope->_attack_rate = OO_SAMPLE_RATE / envelope->attack;
	envelope->_decay_rate = OO_SAMPLE_RATE / envelope->decay;
	envelope->_release_rate = OO_SAMPLE_RATE / envelope->release;
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

void acrylic_envelope_trigger(acrylic_envelope_state_t* state, acrylic_envelope_t *envelope)
{
    state->mode = ACRYLIC_OSCILLATOR_MODE_ATTACK;
}

void acrylic_envelope_release(acrylic_envelope_state_t* state, acrylic_envelope_t *envelope)
{
    state->mode = ACRYLIC_OSCILLATOR_MODE_RELEASE;
}
