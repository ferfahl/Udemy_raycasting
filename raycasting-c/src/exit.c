#include "constants.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern color_t*	colorBuffer;
extern SDL_Texture*	colorBufferTexture;

void	destroyWindow(void)
{
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
