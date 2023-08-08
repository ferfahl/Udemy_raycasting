#include "constants.h"

extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Renderer* renderer;
extern color_t* colorBuffer;
extern SDL_Texture* colorBufferTexture;
extern color_t* textures[NUM_TEXTURES];

void	changeColorIntensity(color_t* color, float factor)
{
	color_t	a = (*color & 0xFF000000);
	color_t	r = (*color & 0x00FF0000) * factor;
	color_t	g = (*color & 0x0000FF00) * factor;
	color_t	b = (*color & 0x000000FF) * factor;

	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

}

void	generate3DProjection(void)
{
	for (int i = 0; i < NUM_RAYS; i++)
	{
		float	correctDist = rays[i].distance* cos(rays[i].rayAngle - player.rotationAngle);
		float	distProjecPlane = (M_WIDTH / 2) / tan(FOV_ANGLE / 2);
		float	projectWallHeight = (TILE_SIZE / correctDist) * distProjecPlane;

		int	wallStripHeight = (int)projectWallHeight;

		int	wallTopPixel = (W_HEIGHT / 2) - (wallStripHeight / 2);
		if (wallTopPixel < 0)
			wallTopPixel = 0;
		
		int wallBotPixel = (W_HEIGHT / 2) + (wallStripHeight / 2);
		if (wallBotPixel > W_HEIGHT)
			wallBotPixel = W_HEIGHT;

		//paint ceiling
		// for (int y = 0; y < wallTopPixel; y++)
		// {
		// 	drawPixel(i, y, 0xFF444444);
		// }
		
		int	offsetX;
		//render the wall from limits above
		if (rays[i].wasHitVertical)
			offsetX = (int)rays[i].wallHitY % TILE_SIZE;
		else
			offsetX = (int)rays[i].wallHitX % TILE_SIZE;

		int	texNbr = rays[i].wallHitContent - 1;

		for (int y = wallTopPixel; y < wallBotPixel; y++)
		{
			int	distFromTop = (y + (wallStripHeight / 2) - (W_HEIGHT / 2));
			int offsetY = distFromTop * ((float)TEXTURE_H / wallStripHeight);
			//set color based on texture
			color_t texelColor = textures[texNbr][(TEXTURE_W * offsetY) + offsetX];

		//makes the pixel darker if ray was hit vertical
			if (rays[i].wasHitVertical)
			{
				changeColorIntensity(&texelColor, 0.7);
			}
			drawPixel(i, y, texelColor);
			// colorBuffer[(M_WIDTH * y) + i] = texelColor;
		}
		// set the color of the floor
        for (int y = wallBotPixel; y < W_HEIGHT; y++) {
            drawPixel(i, y, 0xFF888888);
        }
	}
}

void	clearColorBuffer(color_t color)
{
	for (int i = 0; i < W_WIDTH * W_HEIGHT; i++)
	{
		colorBuffer[i] = color;
	}
}

void	renderColorBuffer(void)
{
	SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)((color_t)W_WIDTH * sizeof(color_t)));
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void	render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//create 3d projection
	generate3DProjection();

	renderColorBuffer();
	clearColorBuffer(0xFF000000);

	//minimap:
	renderMap();
	renderRays();
	renderPlayer();

	SDL_RenderPresent(renderer);
}
