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
