//================================================================================
//27.03.2023
//================================================================================

#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "AVX2_Mandelbrot.h"


//================================================================================

using namespace sf;

//================================================================================

int main ()
{
    SRender render = {};

    render.draw_permission = true;
    render.SIMD_mode = true;
    render.color = black_n_white;
    render_init (&render);

	RenderWindow window(VideoMode(render.W, render.H), "Mandelbrot-Menge");

	Image image;
    image.create(render.W, render.H);

	Texture texture;
	Sprite sprite;

	Font font;
    font.loadFromFile("arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::Green);
    text.setOutlineThickness(4);

    Clock clock;

    STimer timer;
    timer.frames_amnt   = 10;
    timer.frames_cnt    = 0;
    timer.fps           = 0;

	while (window.isOpen())
	{
        start_fps_count (&clock, &timer);

        Event event;
        while (window.pollEvent(event))
        {
            user_interruption (P_ARGUMENTS, &window);
        }

        count_Mandelbrot (&render, &image);


        if (render.draw_permission == true)
        {
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            window.draw(sprite);
        }

        int fps = get_fps_count (&clock, &timer);

        if (fps > 1e-5)
        {
            user_set_display (&text, &render, fps);
        }

        window.draw(text);
        window.display();
    }

    return 0;
}

//================================================================================
