//================================================================================
//29.02.2023
//================================================================================

#include <SFML/Graphics.hpp>

#include "AVX2_Mandelbrot.h"

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

    render->squared_distance = 100;

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

double get_fps_count (sf::Clock* clock, STimer* timer)
{
    if (timer->frames_cnt == timer->frames_amnt)
    {
        timer->fps =  1 / ((double) clock->getElapsedTime().asSeconds() / (double) timer->frames_amnt);

        printf("avg_FPS: %g\n", timer->fps);

        timer->frames_cnt = 0;

        return timer->fps;
    }
    else
    {
        timer->frames_cnt++;
    }

    return 0;
}

//================================================================================

void user_set_display (sf::Text* text, SRender* render, double fps)
{
    char str[100];

    sprintf(str,    "   max iteration:%d\n"
                    "   zoom:x%2.2lf\n"
                    "   FPS:%g", render->max_cnt, render->scale, fps);

    text->setString(str);

    return;
}

void count_Mandelbrot (SRender* render, Image* image)
{
    if (render->SIMD_mode == true)
    {
        for (int y = 0; y < render->H; y++)
        {
            double delta_y  = render->delta_Y / (double) render->H;
            double start_y  = render->min_Y + y * delta_y;

            __m256d y_0 = _mm256_set1_pd (start_y);

            for (int x = 0; x < render->W; x += 4) //4 is hardcoded everywhere
            {
                double delta_x  = render->delta_X / (double) render->W;
                double start_x  = render->min_X + x * delta_x;

                __m256d x_0 = _mm256_add_pd (_mm256_set1_pd (start_x),_mm256_mul_pd (_mm256_set1_pd (delta_x), _mm256_set_pd (0, 1, 2, 3)));

                __m256d cur_x = x_0;
                __m256d cur_y = y_0;

                __m256d squared_y;
                __m256d squared_x;
                __m256d doubled_xy;

                __m256i iterations = _mm256_set1_epi64x (0);

                for (unsigned int cnt = 0; cnt < render->max_cnt; cnt++)
                {
                    squared_x   = _mm256_mul_pd (cur_x, cur_x);
                    squared_y   = _mm256_mul_pd (cur_y, cur_y);

                    doubled_xy  = _mm256_mul_pd (cur_x, cur_y);
                    doubled_xy  = _mm256_add_pd (doubled_xy, doubled_xy);

                    cur_x = _mm256_add_pd (_mm256_sub_pd (squared_x, squared_y), x_0);
                    cur_y = _mm256_add_pd (doubled_xy, y_0);

                    __m256d cmp = _mm256_cmp_pd (_mm256_add_pd (squared_x, squared_y), _mm256_set1_pd (render->squared_distance), _CMP_LT_OQ);
                    bool mask = _mm256_movemask_pd (cmp);

                    //printf ("=%d=\n", mask);
                    if (!mask)
                    {
                        //printf ("====%d=\n", mask);
                        break;
                    }

                    iterations = _mm256_sub_epi64 (iterations, _mm256_castpd_si256(cmp));
                }

                long int* iterations_unpacked = (long int*) &iterations;

                if (render->draw_permission == true)
                {
                    for (int cnt = 0; cnt < 4; cnt++)
                    {
                        //printf ("%ld ", iterations_unpacked[cnt]);
                        image->setPixel (x + cnt, y, paint_it (render, iterations_unpacked[cnt]));
                    }
                    //printf ("%d %d\n", x, y);
                }
            }
        }

        return;
    }

    no_sse_count_Mandelbrot (render, image);

    return;
}

sf::Color paint_it (SRender* render, long int n)
{
    unsigned char monochrome = 255 * !(n % 2);

    if (n == render->max_cnt)
    {
        monochrome = 0;
    }

    return sf::Color{monochrome, monochrome, monochrome};
}

void no_sse_count_Mandelbrot (SRender* render, Image* image)
{
    for (unsigned int y = 0; y < render->H; y++)
    {
        double y_0   = render->min_Y + render->delta_Y * ((double) y / (double) render->H);

        for (unsigned int x = 0; x < render->W; x++)
        {
            double x_0   = render->min_X + render->delta_X * ((double) x / (double) render->W);

            double x_i   = x_0;
            double y_i   = y_0;

            double y2    = 0;
            double x2    = 0;
            double xy    = 0;

            unsigned char a = 0;

            unsigned int cnt = 0;
            for (; (cnt < render->max_cnt) && (x2 + y2 < render->squared_distance); cnt++, a++)
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

void change_mode (SRender* render)
{
    render->SIMD_mode = ! render->SIMD_mode;
}

void user_interruption (FUNC_ARGUMENTS, Window* window)
{
    if (event->type == Event::Closed)
    {
        window->close();
    }

    if (event->type == Event::KeyPressed)
    {
        user_move (ARGUMENTS);

        if (event->key.code == Keyboard::O)
        {
            change_mode (render);
        }

        if (event->key.code == Keyboard::N) //NORMAL_SETTINGS
        {
            render_init (render);
        }

        if (event->key.code == Keyboard::T) //TEST SWITCH
        {
            render->draw_permission = !render->draw_permission;
        }

        if (event->key.code == Keyboard::Escape) //EXIT
        {
            window->close();
        }
    }

    if (event->type == Event::MouseWheelScrolled)
    {
        user_max_cnt_change (ARGUMENTS);
    }

    if (event->type == Event::MouseButtonPressed)
    {
        user_zoom (ARGUMENTS);
    }

    return;
}

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
//__m128d _mm_set_pd (double e1, double e0)
//__m128d _mm_add_pd (__m128d a, __m128d b)

void simd_shift_x (SRender* render, double right_shift)
{
    __m128d new_x_coords = _mm_add_pd ( _mm_set_pd (render->min_X, render->max_X),
                                        _mm_set_pd (right_shift, right_shift));

    render->min_X += *((double*) &new_x_coords);
    render->max_X += *((double*) (&new_x_coords + 1));

    return;
}


//================================================================================
