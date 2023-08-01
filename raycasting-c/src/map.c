#include "constants.h"

extern const int	map[MAP_ROWS][MAP_COLS];
extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int	isGameRunning;
extern int	ticksLastFrame;

int	mapHasWallAt(float x, float y)
{
	if (x < 0 || x > W_WIDTH || y < 0 || y > W_HEIGHT)
		return (TRUE);
	//converts the x and y to the grid position
	int	mapGridIndexX = floor(x / TILE_SIZE);
	int	mapGridIndexY = floor(y / TILE_SIZE);
	//checks if the position is a wall returns true if 1 and false if 0
	return (map[mapGridIndexY][mapGridIndexX] != 0);
}

void	renderMap(void)
{
	for (int i = 0 ; i < MAP_ROWS; i++) { 
		for (int j = 0 ; j < MAP_COLS; j++) { 
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 255 : 0;
			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {tileX * MINIMAP_SCALE, tileY * MINIMAP_SCALE, TILE_SIZE * MINIMAP_SCALE, TILE_SIZE * MINIMAP_SCALE};
			SDL_RenderFillRect(renderer, &mapTileRect);
		}
	}
}
