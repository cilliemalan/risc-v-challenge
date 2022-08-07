#pragma once


typedef struct acrylic_limiter {
    float target;
    float factor;
    float target_factor;
} acrylic_limiter_t;

void acrylic_limiter_init(acrylic_limiter_t *a, float target);
void acrylic_limiter_process(acrylic_limiter_t *a, float *samples, int amount);
