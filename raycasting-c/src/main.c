#include "constants.h"

const int	map[MAP_ROWS][MAP_COLS] = {
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1},
			{1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
			{1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
t_player	player;
t_rays		rays[NUM_RAYS];
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int	isGameRunning;
int	ticksLastFrame= 0;

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

void	render()
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	renderMap();
	renderRays();
	renderPlayer();

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