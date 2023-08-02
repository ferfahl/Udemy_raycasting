#include "constants.h"

extern uint32_t* colorBuffer;

void	drawPixel(int x, int y, uint32_t color)
{
	colorBuffer[(W_WIDTH * y) + x] = color;
}

void	drawRect(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i <= (x + width); i++)
	{
		for (int j = 0; j <= (y + height); j++)
		{
			drawPixel(i, j, color);
		}
	}
}