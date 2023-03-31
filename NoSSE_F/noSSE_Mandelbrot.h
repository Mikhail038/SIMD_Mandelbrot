#pragma once

//================================================================================

#include <SFML/Graphics.hpp>

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

    bool draw_permission;

    double distance;

    int zoom_coef;
}SRender;

typedef struct
{
    int frames_amnt;
    double fps;
    int frames_cnt;
}STimer;
//================================================================================

void start_fps_count (sf::Clock* clock, STimer* timer);

void get_fps_count (sf::Clock* clock, STimer* timer);

//================================================================================

void render_init (SRender* render);

//================================================================================

void count_Mandelbrot (SRender* render, sf::Image* image);

//================================================================================

void user_display (sf::Text* text, SRender* render);

//================================================================================

void user_interruption (FUNC_ARGUMENTS, sf::Window* window);

void user_max_cnt_change (FUNC_ARGUMENTS);

void user_zoom (FUNC_ARGUMENTS);

void user_move (FUNC_ARGUMENTS);

void zoom (FUNC_ARGUMENTS, double z);

//================================================================================

void simd_shift_x (SRender* render, double right_shift);

//================================================================================
