//================================================================================
//29.02.2023
//================================================================================

#include <SFML/Graphics.hpp>
#include "Mandelbrot.h"

//================================================================================

using namespace sf;
using namespace std;

//================================================================================

void render_init (SRender* render)
{
    render->min_X = - render->std_coord;
    render->max_X =   render->std_coord;
    render->min_Y = - render->std_coord;
    render->max_Y =   render->std_coord;

    render->delta_X = render->max_X - render->min_X;
    render->delta_Y = render->max_Y - render->min_Y;

    render->max_cnt = render->std_max_cnt;

    render->scale = 1;

    render->speed_coef = 0.2;

    return;
}

//================================================================================

void user_move (FUNC_ARGUMENTS)
{
    double w = render->delta_X * render->speed_coef;
    double h = render->delta_Y * render->speed_coef;

    if (event->key.code == Keyboard::Left)   {render->min_X -= w, render->max_X -= w; }
    if (event->key.code == Keyboard::Up)     {render->min_Y -= h, render->max_Y -= h; }
    if (event->key.code == Keyboard::Down)   {render->min_Y += h, render->max_Y += h; }
    if (event->key.code == Keyboard::Right)  {render->min_X += w, render->max_X += w; }

    return;
}

void user_max_cnt_change (FUNC_ARGUMENTS)
{
    if (event->MouseWheelScrolled)
    {
        if (event->mouseWheelScroll.wheel == Mouse::VerticalWheel)
        {
            if (event->mouseWheelScroll.delta > 0)
            {
                render->max_cnt += 10;
            }
            else render->max_cnt -= 10;

            if (render->max_cnt < 10)
            {
                render->max_cnt = 10;
            }
        }
    }

    return;
}

void user_zoom (FUNC_ARGUMENTS)
{
    //Left Click to ZoomIn
    if (event->mouseButton.button == Mouse::Left)
    {
        zoom(event, render, 5);
        render->scale *= 5;
    }

    //Right Click to ZoomOut
    if (event->mouseButton.button == Mouse::Right)
    {
        zoom(event, render, 1.0 / 5);
        render->scale /= 5;
    }

    return;
}

void zoom (FUNC_ARGUMENTS, double z)
{
    //mouse point will be new center point
    double cr = render->min_X +
    render->delta_X * ((double) event->mouseButton.x / (double) render->W);

    double ci = render->min_Y +
    render->delta_Y * ((double) event->mouseButton.y / (double) render->H);

    //zoom
    double half_delta_X = render->delta_X / 2;

    render->min_X = cr - half_delta_X / z;
    render->max_X = cr + half_delta_X / z;
    render->delta_X = render->max_X - render->min_X;

    double half_delta_Y = render->delta_Y / 2;

    render->min_Y = ci - half_delta_Y / z;
    render->max_Y = ci + half_delta_Y / z;
    render->delta_Y = render->max_Y - render->min_Y;

    return;
}

//================================================================================
