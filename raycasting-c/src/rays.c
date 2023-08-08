#include "constants.h"

extern t_player	player;
extern t_rays		rays[NUM_RAYS];
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int	isGameRunning;
extern int	ticksLastFrame;

int	isFacingDown(float angle)
{
	if (angle > 0 && angle < PI)
		return (TRUE);
	return (FALSE);
}

int	isFacingUp(float angle)
{
	if (!isFacingDown(angle))
		return (TRUE);
	return (FALSE);
}

int	isFacingRight(float angle)
{
	if (angle < 0.5 * PI || angle > 1.5 * PI)
		return (TRUE);
	return (FALSE);
}

int	isFacingLeft(float angle)
{
	if (!isFacingRight(angle))
		return (TRUE);
	return (FALSE);
}

void	castRay(float rayAngle, int id)
{
	rayAngle = normalizeAngle(rayAngle);

	int	isRayFacingDown = isFacingDown(rayAngle);
	int	isRayFacingUp = isFacingUp(rayAngle);
	int	isRayFacingRight = isFacingRight(rayAngle);
	int	isRayFacingLeft = isFacingLeft(rayAngle);

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
	while (nextHorzTouchX >= 0 && nextHorzTouchX <= M_WIDTH && nextHorzTouchY >= 0 && nextHorzTouchY <= M_HEIGHT) {
		float	xCheck = nextHorzTouchX;
		float	yCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);
		
		if (mapHasWallAt(xCheck, yCheck))
		{
			//found a wall
			foundHorzWallHit = TRUE;
			horzWallHitX = nextHorzTouchX;
			horzWallHitY = nextHorzTouchY;
			horzWallContent = getMapAt((int)floor(yCheck / TILE_SIZE), (int)floor(xCheck / TILE_SIZE));
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
	while (nextVertTouchX >= 0 && nextVertTouchX <= M_WIDTH && nextVertTouchY >= 0 && nextVertTouchY <= M_HEIGHT) {
		float	xCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
		float	yCheck = nextVertTouchY;
		
		if (mapHasWallAt(xCheck, yCheck))
		{
			//found a wall
			foundVertWallHit = TRUE;
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = getMapAt((int)floor(yCheck / TILE_SIZE), (int)floor(xCheck / TILE_SIZE));
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
		horzHitDistance = FLT_MAX;
	if (foundVertWallHit)
		vertHitDistance =  distanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY);
	else
		vertHitDistance = FLT_MAX;
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
	float	distProjPlane = (M_WIDTH / 2) / tan(FOV_ANGLE / 2);

	for (int col= 0; col < NUM_RAYS; col++)
	{
		float angle = player.rotationAngle + atan((col - NUM_RAYS/2) / distProjPlane);

		castRay(angle, col);
	}
}

void	renderRays(void)
{
	for (int i = 0; i < NUM_RAYS; i += 50)
	{
		drawLine(player.x * MINIMAP_SCALE, player.y * MINIMAP_SCALE,
		rays[i].wallHitX * MINIMAP_SCALE, rays[i].wallHitY * MINIMAP_SCALE, 0xFFFF0000);
	}
}
