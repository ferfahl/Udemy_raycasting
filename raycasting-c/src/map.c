#include "constants.h"

static const int map[MAP_ROWS][MAP_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}
};
extern SDL_Renderer* renderer;

int	mapHasWallAt(float x, float y)
{
	if (x < 0 || x > M_WIDTH || y < 0 || y > W_HEIGHT)
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
			
			uint32_t tileColor = map[i][j] != 0 ? 255 : 0;
			// uint32_t tileColor = map[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;
			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {tileX * MINIMAP_SCALE, tileY * MINIMAP_SCALE, TILE_SIZE * MINIMAP_SCALE, TILE_SIZE * MINIMAP_SCALE};
			SDL_RenderFillRect(renderer, &mapTileRect);
			// drawRect(tileX * MINIMAP_SCALE, tileY * MINIMAP_SCALE,
			// 	TILE_SIZE * MINIMAP_SCALE, TILE_SIZE * MINIMAP_SCALE, tileColor);
		}
	}
}

int	getMapAt(int x, int y)
{
	return (map[x][y]);
}
