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

const int   range2  = 400;

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

				if (event.key.code == Keyboard::N)
                {
                    render.min_X = - render.std_coord;
                    render.max_X =   render.std_coord;
                    render.min_Y = - render.std_coord;
                    render.max_Y =   render.std_coord;

                    render.delta_X = render.max_X - render.min_X;
                    render.delta_Y = render.max_Y - render.min_Y;

                    render.max_cnt = render.std_max_cnt;
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

        for (unsigned int y = 0; y < render.H; y++)
        {
            for (unsigned int x = 0; x < render.W; x++)
            {
                double x_0   = render.min_X + render.delta_X * ((double) x / (double) render.W);
                double y_0   = render.min_Y + render.delta_Y * ((double) y / (double) render.H);

                double x_i   = x_0;
                double y2    = 0;
                double x2    = 0;
                double xy    = 0;
                double y_i   = y_0;

                unsigned char a = 0;

                unsigned int cnt = 0;
                for (; cnt < render.max_cnt; cnt++)
                {
                    x2  = x_i * x_i;
                    y2  = y_i * y_i;
                    xy  = x_i * y_i;

                    x_i = x2 - y2 + x_0;
                    y_i = 2 * xy + y_0;

                    if (x2 + y2 > range2)
                    {
                        a = cnt * 7;
                        break;
                    }
                }

                //image.setPixel(x, y, {render.red * (a % 2), render.green * (a % 2) , render.blue * (a % 2)});
                image.setPixel(x, y, {255 * (a % 2), 255 * (a % 2) , 255 * (a % 2)});
            }
        }

        texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);

		char str[100];
		sprintf(str, "max iter:%d\n""zoom:x%2.2lf", render.max_cnt, 1);
		text.setString(str);
		window.draw(text);

		window.display();
    }

}
