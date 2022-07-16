#pragma once

typedef struct acrylic_envelope
{
    // attack duration in seconds.
    float attack;
    // decay time in seconds (aprox)
    float decay;
    // sustain amplitude
    float sustain;
    // release time in seconds (aprox)
    float release;
    
	// precomputed values
	float _attack_rate;
	float _decay_rate;
	float _release_rate;
} acrylic_envelope_t;

typedef enum acrylic_envelope_mode
{
    ACRYLIC_OSCILLATOR_MODE_OFF,
    ACRYLIC_OSCILLATOR_MODE_ATTACK,
    ACRYLIC_OSCILLATOR_MODE_DECAY,
    ACRYLIC_OSCILLATOR_MODE_SUSTAIN,
    ACRYLIC_OSCILLATOR_MODE_RELEASE,
} acrylic_envelope_mode_t;

typedef struct acrylic_envelope_state
{
    acrylic_envelope_mode_t mode;
    float level;
} acrylic_envelope_state_t;

/** @brief Calculates precomputed values for an envelope. */
void acrylic_envelope_initialize(acrylic_envelope_t *envelope);

/** @brief ticks an envelope for one sample.
 *  @returns the current level of the envelope.
 */
float acrylic_envelope_tick(acrylic_envelope_state_t* state, acrylic_envelope_t *envelope);

/** @brief trigger an envelope as if a note was struck */
void acrylic_envelope_trigger(acrylic_envelope_state_t* state, acrylic_envelope_t *envelope);

/** @brief release an envelope as if a note was released */
void acrylic_envelope_release(acrylic_envelope_state_t* state, acrylic_envelope_t *envelope);
