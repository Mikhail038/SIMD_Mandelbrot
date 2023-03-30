//================================================================================
//29.02.2023
//================================================================================

#include <SFML/Graphics.hpp>

#include <immintrin.h>
#include <x86intrin.h>

#include "Mandelbrot.h"

//================================================================================

using namespace sf;
using namespace std;

//================================================================================

void render_init (SRender* render)
{
    int std_coord = 2;

    render->min_X = - std_coord;
    render->max_X =   std_coord;
    render->min_Y = - std_coord;
    render->max_Y =   std_coord;

    render->delta_X = render->max_X - render->min_X;
    render->delta_Y = render->max_Y - render->min_Y;

    render->max_cnt = 256;

    render->scale = 1;

    render->speed_coef = 0.2;

    render->draw_permission = false;

    render->distance = 100;

    render->zoom_coef = 3;

    return;
}

//================================================================================

void start_fps_count (sf::Clock* clock, STimer* timer)
{
    if (timer->frames_cnt == 0)
    {
        clock->restart();
    }

    return;
}

void get_fps_count (sf::Clock* clock, STimer* timer)
{
    if (timer->frames_cnt == timer->frames_amnt)
    {
        timer->fps =  1 / ((double) clock->getElapsedTime().asSeconds() / (double) timer->frames_amnt);

        printf("avg_FPS: %g\n", timer->fps);

        timer->frames_cnt = 0;
    }
    else
    {
        timer->frames_cnt++;
    }

    return;
}

//================================================================================

void user_display (sf::Window* window, sf::Text* text, SRender* render)
{
    char str[100];

    sprintf(str,    "   max iteration:%d\n"
                    "   zoom:x%2.2lf\n", render->max_cnt, render->scale);

    text->setString(str);

    return;
}

void count_Mandelbrot (SRender* render, Image* image)
{
    for (unsigned int y = 0; y < render->H; y++)
    {
        for (unsigned int x = 0; x < render->W; x++)
        {
            double x_0   = render->min_X + render->delta_X * ((double) x / (double) render->W);
            double y_0   = render->min_Y + render->delta_Y * ((double) y / (double) render->H);

            double x_i   = x_0;
            double y_i   = y_0;

            double y2    = 0;
            double x2    = 0;
            double xy    = 0;

            unsigned char a = 0;

            unsigned int cnt = 0;
            for (; (cnt < render->max_cnt) && (x2 + y2 < render->distance); cnt++, a++)
            {
                x2  = x_i * x_i;
                y2  = y_i * y_i;
                xy  = x_i * y_i;

                x_i = x2 - y2 + x_0;
                y_i = 2 * xy + y_0;
            }


            if (render->draw_permission == true)
            {
                a *= 7;

                //image->setPixel(x, y, {render->red * (a % 2), render->green * (a % 2) , render->blue * (a % 2)});
                image->setPixel(x, y, {255 * (a % 2), 255 * (a % 2) , 255 * (a % 2)});
            }
        }
    }

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
