#pragma once

//================================================================================

#include <SFML/Graphics.hpp>

//================================================================================

#define FUNC_ARGUMENTS sf::Event* event, SRender* render

#define ARGUMENTS &event, &render


//================================================================================

typedef struct
{
    const int   W   = 1080;
    const int   H   = 1080;

    const int   std_max_cnt = 256;


    const double std_coord = 2;

    double min_X;
    double max_X;
    double delta_X;

    double min_Y;
    double max_Y;
    double delta_Y;

    double speed_coef;

    int max_cnt;

    double scale;

}SRender;

//================================================================================

void render_init (SRender* render);

//================================================================================

void user_max_cnt_change (FUNC_ARGUMENTS);

void user_zoom (FUNC_ARGUMENTS);

void user_move (FUNC_ARGUMENTS);

void zoom (FUNC_ARGUMENTS, double z);
//================================================================================


