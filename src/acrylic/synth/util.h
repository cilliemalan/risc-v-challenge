#pragma once

/** fast 10^x */
float apow10(float input);
/** fast 1/x */
float areciprocal(float input);

/** interpolation */
float alerp(float a, float b, float t);
float aglerp(float a, float b, float t, float g);
float aelerp(float a, float b, float t);

float afsinf(float t);
float afcosf(float t);

/** @brief convert a level (from 0 to 1) to an amplitude.
 *  This treats level 0 as -45db (the 16 bit SNR)
 */
static inline float level_to_amplitude(float level)
{
    float db = (level - 1) * 4.5f;
    return apow10(db);
}
