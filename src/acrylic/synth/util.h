#pragma once

float apow10(float input);
float alerp(float a, float b, float t);
float aglerp(float a, float b, float t, float g);
float aelerp(float a, float b, float t);
float adecay(float a, float b, float rate);
float aattack(float a, float d, float s, float t);
float arelease(float t, float s, float r);

/** convert a level (from 0 to 1) to an amplitude */
static inline float level_to_amplitude(float level)
{
    float db = (level - 1) * 4.5f;
    return apow10(db);
}
