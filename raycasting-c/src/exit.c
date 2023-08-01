#include "constants.h"

extern const int	map[MAP_ROWS][MAP_COLS];
extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int	isGameRunning;
extern int	ticksLastFrame;

void	destroyWindow(void)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
