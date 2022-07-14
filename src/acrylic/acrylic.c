#include "acrylic.h"
#include <memory.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

struct acrylic
{
    acrylic_config_t config;
};

acrylic_t acrylic_new(acrylic_config_t *config)
{
    acrylic_t a = (acrylic_t)calloc(1, sizeof(struct acrylic));
    memcpy(&a->config, config, sizeof(acrylic_config_t));
    return a;
}

void acrylic_event(acrylic_t a, acrylic_event_type_t type, acrylic_event_t *event)
{
}

void acrylic_process(acrylic_t a, float *data, int num_samples)
{
}

void acrylic_free(acrylic_t instance)
{
    free(instance);
}
