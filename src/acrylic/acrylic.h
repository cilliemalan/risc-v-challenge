#pragma once

typedef struct acrylic_config
{
    struct _lv_disp_t *lvgl_handle;
} acrylic_config_t;

typedef enum acrylic_event_type
{
    ACRYLIC_EVENT_TYPE_BUTTON_DOWN,
    ACRYLIC_EVENT_TYPE_BUTTON_UP,
    ACRYLIC_EVENT_TYPE_ENCODER,
} acrylic_event_type_t;

typedef enum acrylic_encoder
{
    ACRYLIC_ENCODER_1,
    ACRYLIC_ENCODER_2,
} acrylic_encoder_t;

typedef enum acrylic_direction
{
    ACRYLIC_DIRECTION_LEFT,
    ACRYLIC_DIRECTION_RIGHT,
} acrylic_direction_t;

typedef union acrylic_event
{
    struct
    {
        char number;
    } button;
    struct
    {
        acrylic_encoder_t encoder;
        acrylic_direction_t direction;
    } encoder;
} acrylic_event_t;

typedef struct acrylic *acrylic_t;

acrylic_t acrylic_new(acrylic_config_t *config);
void acrylic_event(acrylic_t a, acrylic_event_type_t type, acrylic_event_t *event);
void acrylic_process(acrylic_t a, float *data, int num_samples);
void acrylic_free(acrylic_t a);
