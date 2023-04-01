#pragma once

//================================================================================

#include <SFML/Graphics.hpp>

#include <immintrin.h>
#include <x86intrin.h>

//================================================================================

#define FUNC_ARGUMENTS sf::Event* event, SRender* render

#define ARGUMENTS event, render
#define P_ARGUMENTS &event, &render

//================================================================================

typedef struct
{
    const int   W   = 1080;
    const int   H   = 1080;

    double min_X;
    double max_X;
    double delta_X;

    double min_Y;
    double max_Y;
    double delta_Y;

    double speed_coef;

    int max_cnt;

    double scale;

    volatile bool draw_permission;

    double squared_distance;

    int zoom_coef;

    bool SIMD_mode;
}SRender;

typedef struct
{
    int frames_amnt;
    double fps;
    int frames_cnt;
}STimer;
//================================================================================

void start_fps_count (sf::Clock* clock, STimer* timer);

double get_fps_count (sf::Clock* clock, STimer* timer);

//================================================================================

void render_init (SRender* render);

//================================================================================

void user_set_display (sf::Text* text, SRender* render, double fps);

//================================================================================

void count_Mandelbrot (SRender* render, sf::Image* image);

sf::Color paint_it (SRender* render, long int n);

bool distance_cmp (__m256d squared_x, __m256d squared_y, SRender* render);

void no_sse_count_Mandelbrot (SRender* render, sf::Image* image);

//================================================================================

void user_interruption (FUNC_ARGUMENTS, sf::Window* window);

void user_max_cnt_change (FUNC_ARGUMENTS);

void user_zoom (FUNC_ARGUMENTS);

void user_move (FUNC_ARGUMENTS);

void zoom (FUNC_ARGUMENTS, double z);

//================================================================================

void simd_shift_x (SRender* render, double right_shift);

//================================================================================
