#include "raylib.h"
#include "lvgl.h"
#include <math.h>
#include <assert.h>
#include "acrylic.h"

#define screenWidth 800
#define screenHeight 450
#define dispWidth 240
#define dispHeight 240
#define sw2 (screenWidth / 2)
#define sh2 (screenHeight / 2)

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif
#ifdef _MSC_VER
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

static struct buttondata
{
    float x;
    float y;
    float r;
    Color c;
    bool mouse_down;
    bool key_down;
    bool is_down;
} buttons[16 + 6 + 2];

static int buttonkeys[16 + 6 + 2] =
    {
        KEY_A, // C
        KEY_W, // C#
        KEY_S, // D
        KEY_E, // D#
        KEY_D, // E
        KEY_F, // F
        KEY_T, // F#
        KEY_G, // G
        KEY_Y, // G#
        KEY_H, // A
        KEY_U, // A#
        KEY_J, // B
        KEY_K, // C
        KEY_O, // C#
        KEY_L, // D
        KEY_P, // D#

        KEY_Z,          // l1
        KEY_X,          // l2
        KEY_C,          // l3
        KEY_LEFT_SHIFT, // LE

        KEY_COMMA,       // r1
        KEY_PERIOD,      // r2
        KEY_SLASH,       // r3
        KEY_RIGHT_SHIFT, // RE
};

static Texture displayTexture;
static lv_disp_t *lvgl_display;
static acrylic_t acrylic;

static Vector2 circle_points[128];
static Vector2 circle_texcoords[128];

static bool mouse_button_was_down = false;
static bool mouse_button_is_down = false;

static void button_pressed(int button)
{
    acrylic_event_t e;
    e.button.number = button;
    acrylic_event(acrylic, ACRYLIC_EVENT_TYPE_BUTTON_DOWN, &e);
}

static void encoder_moved(int encoder, int direction)
{
    acrylic_event_t e;
    e.encoder.encoder = (acrylic_encoder_t)encoder;
    e.encoder.direction = (acrylic_direction_t)direction;
    acrylic_event(acrylic, ACRYLIC_EVENT_TYPE_ENCODER, &e);
}

static void button_released(int button)
{
    acrylic_event_t e;
    e.button.number = button;
    acrylic_event(acrylic, ACRYLIC_EVENT_TYPE_BUTTON_UP, &e);
}

static void init_window()
{
    InitWindow(screenWidth, screenHeight, "Acrylic");
    SetTargetFPS(60);
}

static void lvgl_drv_flush(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    assert(sizeof(*color_p) == 4);

    Rectangle ur =
        {
            area->x1,
            area->y1,
            area->x2 - area->x1 + 1,
            area->y2 - area->y1 + 1,
        };
    UpdateTextureRec(displayTexture, ur, color_p);
    lv_disp_flush_ready(disp_drv);
}

static void init_display_buffer()
{
    Image image = GenImageColor(dispWidth, dispHeight, DARKBLUE);
    displayTexture = LoadTextureFromImage(image);
    UnloadImage(image);
}

static void init_circle()
{
    for (int i = 0; i < sizeof(circle_points) / sizeof(circle_points[0]); i++)
    {
        float t = 2.0f * i * PI / 127.0f;
        float x = -cosf(t);
        float y = sinf(t);
        circle_points[i].x = x * dispWidth * 0.5f;
        circle_points[i].y = y * dispHeight * 0.5f;
        circle_texcoords[i].x = 0.5f * (x + 1.0f);
        circle_texcoords[i].y = 0.5f * (y + 1.0f);
    }
}

static void init_lvgl()
{
    lv_init();

    static lv_disp_draw_buf_t draw_buffer;
    static lv_color_t buffer1[5 * dispWidth];
    static lv_color_t buffer2[5 * dispWidth];
    lv_disp_draw_buf_init(
        &draw_buffer,
        buffer1,
        buffer2,
        sizeof(buffer1) / sizeof(buffer1[0]));

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = dispWidth;
    disp_drv.ver_res = dispWidth;

    disp_drv.flush_cb = lvgl_drv_flush;
    disp_drv.draw_buf = &draw_buffer;
    lv_disp_drv_register(&disp_drv);
}

static void init_acrylic()
{
    acrylic_config_t config;
    config.lvgl_handle = lvgl_display;
    acrylic = acrylic_new(&config);
}

static void init_display()
{
    init_display_buffer();
    init_circle();
    init_lvgl();
}

static void init_buttons()
{
    for (int i = 0; i < 16; i++)
    {
        float t = i * (PI * 15.0f / 180.0f) - (1.5f * PI * 15.0f / 180.0f);
        buttons[i].x = -cosf(t) * 160.0f + sw2;
        buttons[i].y = sinf(t) * 160.0f + sh2;
        buttons[i].r = 20.0f;
        buttons[i].c = DARKGRAY;
    }

    for (int i = 0; i < 3; i++)
    {
        float t = i * (PI * 7.5f / 180.0f) - (3.5f * PI * 15.0f / 180.0f);
        buttons[2 - i + 16].x = -cosf(t) * 160.0f + sw2;
        buttons[2 - i + 16].y = sinf(t) * 160.0f + sh2;
        buttons[2 - i + 16].r = 10.0f;
        buttons[2 - i + 16].c = DARKGRAY;
        buttons[16 + 4 + i].x = cosf(t) * 160.0f + sw2;
        buttons[16 + 4 + i].y = sinf(t) * 160.0f + sh2;
        buttons[16 + 4 + i].r = 10.0f;
        buttons[16 + 4 + i].c = DARKGRAY;
    }

    {
        float t = -(3.0f * PI * 15.0f / 180.0f);
        buttons[16 + 3].x = -cosf(t) * 190.0f + sw2;
        buttons[16 + 3].y = sinf(t) * 190.0f + sh2;
        buttons[16 + 3].r = 20.0f;
        buttons[16 + 3].c = DARKGRAY;
        buttons[16 + 4 + 3].x = cosf(t) * 190.0f + sw2;
        buttons[16 + 4 + 3].y = sinf(t) * 190.0f + sh2;
        buttons[16 + 4 + 3].r = 20.0f;
        buttons[16 + 4 + 3].c = DARKGRAY;
    }
}

static void audio_callback(void *buffer, unsigned int frames)
{
    float tmp[48];
    const int tmpsize = sizeof(tmp) / sizeof(tmp[0]);
    uint16_t *b = (uint16_t *)buffer;

    while (frames > 0)
    {
        int amt = frames > tmpsize ? tmpsize : frames;
        acrylic_process(acrylic, tmp, amt);

        for (int i = 0; i < amt; i++)
        {
            b[i] = (uint16_t)(tmp[i] * 32767.0f);
        }

        b += amt;
        frames -= amt;
    }
}

static void init_audio()
{
    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(240);
    AudioStream stream = LoadAudioStream(48000, 16, 1);
    SetAudioStreamCallback(stream, audio_callback);
    PlayAudioStream(stream);
}

static void do_buttons()
{
    Vector2 mp = GetMousePosition();
    float mx = mp.x;
    float my = mp.y;
    int numbuttons = sizeof(buttons) / sizeof(buttons[0]);
    mouse_button_is_down = IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    for (int i = 0; i < numbuttons; i++)
    {
        if (mouse_button_is_down != mouse_button_was_down)
        {
            if (mouse_button_is_down)
            {
                float bx = buttons[i].x;
                float by = buttons[i].y;
                float br = buttons[i].r;
                float sqdist = (bx - mx) * (bx - mx) + (by - my) * (by - my);
                float sqr = br * br;
                if (sqdist <= sqr)
                {
                    buttons[i].mouse_down = true;
                }
            }
            else
            {
                if (buttons[i].mouse_down)
                {
                    button_released(i);
                    buttons[i].mouse_down = false;
                }
            }
        }

        buttons[i].key_down = IsKeyDown(buttonkeys[i]);

        bool is_down = buttons[i].mouse_down || buttons[i].key_down;

        if (buttons[i].is_down != is_down)
        {
            buttons[i].is_down = is_down;
            if (is_down)
            {
                button_pressed(i);
            }
            else
            {
                button_released(i);
            }
        }
    }
}

static void do_encoders()
{
    bool e1l = IsKeyPressed(KEY_LEFT_CONTROL);
    bool e1r = IsKeyPressed(KEY_LEFT_ALT);
    bool e2l = IsKeyPressed(KEY_RIGHT_ALT);
    bool e2r = IsKeyPressed(KEY_RIGHT_CONTROL);

    if (e1l && !IsKeyDown(KEY_LEFT_ALT))
        encoder_moved(0, 0);
    if (e1r && !IsKeyDown(KEY_LEFT_CONTROL))
        encoder_moved(0, 1);
    if (e2l && !IsKeyDown(KEY_RIGHT_CONTROL))
        encoder_moved(1, 0);
    if (e2r && !IsKeyDown(KEY_RIGHT_ALT))
        encoder_moved(1, 1);
}

static void do_draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    Vector2 sc = {sw2, sh2};
    DrawTexturePoly(displayTexture,
                    sc,
                    circle_points,
                    circle_texcoords,
                    sizeof(circle_points) / sizeof(circle_points[0]),
                    WHITE);
    DrawCircleLines(screenWidth / 2, screenHeight / 2, dispWidth / 2, BLACK);

    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
    {
        float x = buttons[i].x;
        float y = buttons[i].y;
        float r = buttons[i].r;
        Color c = buttons[i].c;
        if (buttons[i].is_down)
        {
            DrawCircleGradient(x, y, r, RAYWHITE, c);
        }
        else
        {
            DrawCircle(x, y, r, c);
        }
    }

    EndDrawing();
}

int main()
{
    init_window();
    init_display();
    init_buttons();
    init_audio();

    while (!WindowShouldClose())
    {
        lv_tick_inc((uint32_t)(GetFrameTime() * 1000));
        lv_task_handler();

        do_buttons();
        do_encoders();
        do_draw();
    }

    CloseWindow();
    return 0;
}
