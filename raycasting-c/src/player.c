#include "constants.h"

extern t_player	player;
extern SDL_Renderer* renderer;


void	movePlayer(float deltaTime)
{
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaTime;

	float	moveStep = player.walkDirection * player.walkSpeed * deltaTime;
	float	posPlayX = player.x + cos(player.rotationAngle) * moveStep;
	float	posPlayY = player.y + sin(player.rotationAngle) * moveStep;

	//wall collision
	if (!mapHasWallAt(posPlayX, posPlayY))
	{
		player.x = posPlayX;
		player.y = posPlayY;
	}
}

void	renderPlayer(void)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect playerRect = {player.x * MINIMAP_SCALE, player.y * MINIMAP_SCALE, player.width * MINIMAP_SCALE, player.height * MINIMAP_SCALE};
	SDL_RenderFillRect(renderer, &playerRect);

	SDL_RenderDrawLine(renderer,
		MINIMAP_SCALE * player.x,
		MINIMAP_SCALE * player.y,
		MINIMAP_SCALE * player.x + cos(player.rotationAngle) * 40,
		MINIMAP_SCALE * player.y + sin(player.rotationAngle) * 40);
}
