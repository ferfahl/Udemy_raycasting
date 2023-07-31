#ifndef CONSTANTS_H
# define CONSTANTS_H

# include <stdio.h>
# include <SDL2/SDL.h>

typedef struct s_player
{
	float	x;
	float	y;
	float	width;
	float	height;
	int		turnDirection; //-1 for left, +1 for right
	int		walkDirection; //-1 for back, +1 for front
	float	rotationAngle;
	float	walkSpeed;
	float	turnSpeed;
}				t_player;

# define PI 3.14159265
# define TWO_PI 6.28318530

# define TILE_SIZE 64
# define MAP_ROWS 11
# define MAP_COLS 15

# define MINIMAP_SCALE 0.3

# define FALSE 0
# define TRUE 1

# define W_WIDTH (MAP_COLS * TILE_SIZE)
# define W_HEIGHT (MAP_ROWS * TILE_SIZE)

# define FOV_ANGLE (60 * (PI / 180))

# define NUM_RAYS W_WIDHT

# define FPS 30
# define FRAME_TIME_LENGTH (1000 / FPS)

#endif