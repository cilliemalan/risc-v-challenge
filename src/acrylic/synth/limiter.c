#include "limiter.h"
#include "synth.h"
#include <math.h>

void acrylic_limiter_init(acrylic_limiter_t *a, float target)
{
    a->factor = 1.0f;
    a->target_factor = 1.0f;
    a->target = target;
}

void acrylic_limiter_process(acrylic_limiter_t *a, float *samples, int amount)
{
    for (int i = 0; i < amount; i++)
    {
        float s = samples[i];
        float ns;
        if (a->factor < 1.0f)
        {
            ns = s * a->factor;
        }
        else
        {
            ns = s;
        }

        if (a->target_factor != a->factor)
        {
            float diff = a->target_factor - a->factor;
            a->factor = a->factor + diff * 0.05f;
        }

        if (fabsf(ns) > a->target)
        {
            float nf = fabs(a->target / (s * 1.1f));
            a->target_factor = nf;
        }
        else if (a->factor < 1.0f)
        {
            a->target_factor *= 1.0001;
        }

        samples[i] = ns;
    }
}
