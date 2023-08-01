#include "constants.h"

extern const int	map[MAP_ROWS][MAP_COLS];
extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int	isGameRunning;
extern int	ticksLastFrame;

void	castRay(float rayAngle, int id)
{
	rayAngle = normalizeAngle(rayAngle);

	int	isRayFacingDown;
	int	isRayFacingUp;
	int	isRayFacingRight;
	int	isRayFacingLeft;

	if (rayAngle > 0 && rayAngle < PI)
		isRayFacingDown = TRUE;
	else
		isRayFacingDown = FALSE;
	isRayFacingUp = !isRayFacingDown;

	if (rayAngle < 0.5 * PI || rayAngle > 1.5 * PI)
		isRayFacingRight = TRUE;
	else
		isRayFacingRight = FALSE;
	isRayFacingLeft = !isRayFacingRight;

	float xintercept, yintercept;
	float xstep, ystep;

	// horizontalHit(rayAngle, id);
	int		foundHorzWallHit = FALSE;
	float	horzWallHitX = 0;
	float	horzWallHitY = 0;
	int		horzWallContent = 0;

	//find the closest horizontal grid intersection (x, y)
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	//calculate the increment xstep and ystep (delta)
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(rayAngle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float	nextHorzTouchX = xintercept;
	float	nextHorzTouchY = yintercept;

	//increment xstep and ystep until we find a wall
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= W_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= W_HEIGHT) {
		float	xCheck = nextHorzTouchX;
		float	yCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
		
		if (mapHasWallAt(xCheck, yCheck))
		{
			//found a wall
			foundHorzWallHit = TRUE;
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = map[(int)floor(yCheck / TILE_SIZE)][(int)floor(xCheck / TILE_SIZE)];
			break;
		}
		else
		{
			nextHorzTouchX += xstep;
			nextHorzTouchY += ystep;
		}
	}
	//
	// verticalHit(rayAngle, id);
	int		foundVertWallHit = FALSE;
	float	vertWallHitX = 0;
	float	vertWallHitY = 0;
	int		vertWallContent = 0;

	//find the closest vertical grid intersection (x, y)
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	//calculate the increment xstep and ystep (delta)
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float	nextVertTouchX = xintercept;
	float	nextVertTouchY = yintercept;

	//increment xstep and ystep until we find a wall
	while (nextVertTouchX >= 0 && nextVertTouchX <= W_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= W_HEIGHT) {
		float	xCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float	yCheck = nextVertTouchY;
		
		if (mapHasWallAt(xCheck, yCheck))
		{
			//found a wall
			foundVertWallHit = TRUE;
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = map[(int)floor(yCheck / TILE_SIZE)][(int)floor(xCheck / TILE_SIZE)];
			break;
		}
		else
		{
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}

	float	horzHitDistance;
	float	vertHitDistance;

	if (foundHorzWallHit)
		horzHitDistance = distanceBetweenPoints(player.x, player.y, horzWallHitX, horzWallHitY);
	else
		horzHitDistance = INT_MAX;
	if (foundVertWallHit)
		vertHitDistance =  distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY);
	else
		vertHitDistance = INT_MAX;
	if (vertHitDistance < horzHitDistance)
	{
		rays[id].distance = vertHitDistance;
		rays[id].wallHitX = vertWallHitX;
		rays[id].wallHitY = vertWallHitY;
		rays[id].wasHitVertical = TRUE;
		rays[id].wallHitContent = vertWallContent;
	}
	else
	{
		rays[id].distance = horzHitDistance;
		rays[id].wallHitX = horzWallHitX;
		rays[id].wallHitY = horzWallHitY;
		rays[id].wasHitVertical = FALSE;
		rays[id].wallHitContent = horzWallContent;
	}
	rays[id].rayAngle = rayAngle;
	rays[id].isRayDown = isRayFacingDown;
	rays[id].isRayUp = isRayFacingUp;
	rays[id].isRayLeft = isRayFacingLeft;
	rays[id].isRayRight = isRayFacingRight;
}

void	castAllRays(void)
{
	float	rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
	int		i = 0;

	while (i < NUM_RAYS)
	{
		castRay(rayAngle, i);
		rayAngle += FOV_ANGLE / NUM_RAYS;
		i++;
	}
	
}

void	renderRays(void)
{
	int	i = 0;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	while (i < NUM_RAYS)
	{
		SDL_RenderDrawLine(renderer, player.x * MINIMAP_SCALE, player.y* MINIMAP_SCALE, rays[i].wallHitX * MINIMAP_SCALE, rays[i].wallHitY * MINIMAP_SCALE);
		i++;
	}
}
