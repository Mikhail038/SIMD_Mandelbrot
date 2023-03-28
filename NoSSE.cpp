#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const int   W   = 1080;
const int   H   = 1080;

#define ABS_DELTA 1.5

const float min_X   = -ABS_DELTA;
const float max_X   =  ABS_DELTA;

const float min_Y   = -ABS_DELTA;
const float max_Y   =  ABS_DELTA;

const int   max_iter    = 400;

const int   range   = 400;

Color linear_interpolation(const Color& v, const Color& u, double a)
{
	auto const b = 1 - a;
	return Color(b*v.r + a * u.r, b*v.g + a * u.g, b*v.b + a * u.b);
}

int main ()
{
	RenderWindow window(VideoMode(W, H), "Mandelbrot-Menge");

	Image image;
    image.create(W, H);

	Texture texture;
	Sprite sprite;

	Font font;
    font.loadFromFile("arial.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(Color::White);

	while (window.isOpen())
	{
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
        }

        for (unsigned int y = 0; y < H; y++)
        {
            for (unsigned int x = 0; x < W; x++)
            {
                float x_0   = min_X + (max_X - min_X) * x / W;
                float y_0   = min_Y + (max_Y - min_Y) * y / H;

                float x_i   = x_0;
                float y2    = y_0;
                float x2    = 0;
                float xy    = 0;
                float y_i   = 0;

                unsigned char a = 0;

                unsigned int iter = 0;
                for (; iter < max_iter; iter++)
                {
                    x2  = x_i * x_i;
                    y2  = y_i * y_i;
                    xy  = x_i * y_i;

                    x_i = x2 - y2 + x_0;
                    y_i = 2 * xy + y_0;

                    if (x2 + y2 > range)
                    {
                        a = iter * 10;
                        break;
                    }
                }

                /*
                int r = 1.0 * (max_iter - iter) / max_iter * 0xff;

                int g = r;
                int b = r;


                //color pallet similar to Ultra Fractal and Wikipedia
                static const vector<Color> colors{
                    {0,7,100},
                    {32,107,203},
                    {237,255,255},
                    {255,170,0},
                    {0,2,0},
                };

                static const vector<Color> colors
                {
                    {0,0,0},
                    {213,67,31},
                    {251,255,121},
                    {62,223,89},
                    {43,30,218},
                    {0,255,247}
                };

                static const auto max_color = colors.size() - 1;
                if (iter == max_iter)iter = 0;
                double mu = 1.0*iter / max_iter;
                //scale mu to be in the range of colors
                mu *= max_color;
                auto i_mu = static_cast<size_t>(mu);
                auto color1 = colors[i_mu];
                auto color2 = colors[min(i_mu + 1, max_color)];

                Color c = linear_interpolation(color1, color2, mu - i_mu);
                */

                image.setPixel(x, y, {a, a, a});
            }
        }

        texture.loadFromImage(image);
		sprite.setTexture(texture);
		window.draw(sprite);

		char str[100];
		sprintf(str, "max iter:%d\n""zoom:x%2.2lf", max_iter, 1);
		text.setString(str);
		window.draw(text);

		window.display();
    }

}
