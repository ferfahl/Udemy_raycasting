#include "constants.h"

extern const int	map[MAP_ROWS][MAP_COLS];
extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int	isGameRunning;
extern int	ticksLastFrame;

int	start_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return (FALSE);
	}
	window = SDL_CreateWindow("Raycast", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W_WIDTH, W_HEIGHT, SDL_WINDOW_BORDERLESS);
	if (!window)
	{
		fprintf(stderr, "Error creating window.\n");
		return (FALSE);
	}
	//-1 is the default driver
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error starting render.\n");
		return (FALSE);
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return (TRUE);
}


void	process_input(void)
{
	SDL_Event	event;

	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT: {
			isGameRunning = FALSE;
			break ;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = FALSE;
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = + 1;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = - 1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = + 1;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = - 1;
			break ;
		}
		case SDL_KEYUP: {
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = 0;
			if (event.key.keysym.sym == SDLK_LEFT)
				player.turnDirection = 0;
			break ;
		}
	}
}

void	setup(void)
{
	player.x = W_WIDTH / 2;
	player.y = W_HEIGHT / 2;
	player.width = 1;
	player.height = 1;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2;
	player.walkSpeed = 100;
	player.turnSpeed = 45 * (PI / 180);
	//start objs
}
