#pragma once


typedef struct acrylic_limiter {
    float target;
    float factor;
    float target_factor;
    float decay_rate;
} acrylic_limiter_t;

void acrylic_limiter_init(acrylic_limiter_t *a, float decay_time, float target);
void acrylic_limiter_process(acrylic_limiter_t *a, float *samples, int amount);
