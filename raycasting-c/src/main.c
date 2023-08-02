#include "constants.h"
#include "map.h"

t_player	player;
t_rays		rays[NUM_RAYS];
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int	isGameRunning;
int	ticksLastFrame= 0;
uint32_t* colorBuffer = NULL;
SDL_Texture* colorBufferTexture = NULL;
uint32_t* textures[NUM_TEXTURES];

void	update()
{
	int	timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
		SDL_Delay(timeToWait);
	
	float	deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);
	castAllRays();
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
			uint32_t texelColor = textures[texNbr][(TEXTURE_W * offsetY) + offsetX];
			colorBuffer[(M_WIDTH * y) + i] = texelColor;
		}
	}
	// drawRect(320, 320, 580, 580, 0xFF333333);
}

void	clearColorBuffer(uint32_t color)
{
	for (int x = 0; x < W_WIDTH; x++)
	{
		for (int y = 0; y < W_HEIGHT; y++)
		{
			if (y < W_HEIGHT / 2) //ceiling
				colorBuffer[(W_WIDTH * y) + x] = 0xFF333333;
			else //floor
				colorBuffer[(W_WIDTH * y) + x] = color;
		}
	}
}

void	renderColorBuffer(void)
{
	SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)((uint32_t)W_WIDTH * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void	render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//create 3d projection
	generate3DProjection();

	renderColorBuffer();
	clearColorBuffer(0xFF000000);

	//minimap:
	renderMap();
	// renderRays();
	// renderPlayer();

	SDL_RenderPresent(renderer);
}

int	main(void)
{
	isGameRunning = start_window();
	
	setup();
	while(isGameRunning)
	{
		process_input();
		update();
		render();
	}
	destroyWindow();
	return (0);
}
