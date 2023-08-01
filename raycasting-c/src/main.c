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

int	start_window()
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

void	destroyWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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

void	renderPlayer()
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

float	normalizeAngle(float angle)
{
	angle = remainder(angle, TWO_PI);
	if (angle < 0)
		angle = TWO_PI + angle;
	return (angle);
}

float	distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

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

void	castAllRays()
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

void	renderRays()
{
	int	i = 0;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	while (i < NUM_RAYS)
	{
		SDL_RenderDrawLine(renderer, player.x * MINIMAP_SCALE, player.y* MINIMAP_SCALE, rays[i].wallHitX * MINIMAP_SCALE, rays[i].wallHitY * MINIMAP_SCALE);
		i++;
	}
}

void	renderMap()
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

void	process_input()
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

void	setup()
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