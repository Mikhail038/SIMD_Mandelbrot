//================================================================================
//27.03.2023
//================================================================================

#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.hpp>

#include "Mandelbrot.h"

//================================================================================

using namespace sf;
using namespace std;

//================================================================================

int main ()
{
    SRender render = {};

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


	while (window.isOpen())
	{
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::KeyPressed)
			{
                user_move (ARGUMENTS);

				if (event.key.code == Keyboard::N) //NORMAL_SETTINGS
                {
                    render_init (&render);
                }

                if (event.key.code == Keyboard::T) //TEST
                {
                    render.draw_permission = !render.draw_permission;
                }

				if (event.key.code == Keyboard::Escape)
                {
                    window.close();
                }
			}

            if (event.type == Event::MouseWheelScrolled)
			{
                user_max_cnt_change (ARGUMENTS);
			}

            if (event.type == Event::MouseButtonPressed)
			{
                user_zoom (ARGUMENTS);
			}
        }

        Clock clock;

        start_fps_count (&clock);

        count_Mandelbrot (&render, &image);

        if (render.draw_permission == 1)
        {
            texture.loadFromImage(image);
            sprite.setTexture(texture);
            window.draw(sprite);
        }

        int fps = get_fps_count (&clock);

        if (render.draw_permission == 1)
        {
            user_display (&window, &text, &render, fps);
            window.draw(text);

            window.display();
        }

        printf("FPS: %d\n", fps);

    }

}

//================================================================================
