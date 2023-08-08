#include "constants.h"

extern color_t* colorBuffer;

void	drawPixel(int x, int y, color_t color)
{
	colorBuffer[(W_WIDTH * y) + x] = color;
}

void	drawRect(int x, int y, int width, int height, color_t color)
{
	for (int i = x; i <= (x + width); i++)
	{
		for (int j = y; j <= (y + height); j++)
		{
			drawPixel(i, j, color);
		}
	}
}

void	drawLine(int x0, int y0, int x1, int y1, color_t color)
{
	int	delta_X = (x1 - x0);
	int	delta_Y = (y1 - y0);
	int	sideLenght;

	if (delta_X >= delta_Y)
		sideLenght = abs(delta_X);
	else
		sideLenght = abs(delta_Y);

	float	xIncrement = delta_X / (float) sideLenght;
	float	yIncrement = delta_Y / (float) sideLenght;

	float	current_x = x0;
	float	current_y = y0;
	for (int i = 0; i < sideLenght; i++)
	{
		drawPixel(round(current_x), round(current_y), color);
		current_x += xIncrement;
		current_y += yIncrement;
	}
}