#ifndef WORLD_H
#define WORLD_H

#include <stdlib.h>

#define FULLSCREEN 0

#define MAP_WIDTH  24
#define MAP_HEIGHT 24

#define TILE_SIZE 24

#define SNAKE_VSTART    43
#define SNAKE_VMAX      56
#define SNAKE_SIZE_INIT 3
#define SNAKE_SIZE_MAX  512

#define SNAKE_STATE_OK       0
#define SNAKE_STATE_COLLIDE  1
#define SNAKE_STATE_OUT      2

#define SNAKE_RULE_FLAT  0
#define SNAKE_RULE_TORUS 1
#define SNAKE_RULE SNAKE_RULE_FLAT//TORUS

#define CLIP_COUNT      24
#define CLIP_SNAKE_HEAD 0
#define CLIP_SNAKE_BODY 4
#define CLIP_SNAKE_BEND 8
#define CLIP_SNAKE_TALE 12
#define CLIP_FLOOR_1    16
#define CLIP_BONUS      20

#define BONUS_RESET 0
#define BONUS_RESET_TIME

#define USE_GRAFFIC_BUFFER 0

#define random(n)((int)((float)n *((float)rand()/(float)RAND_MAX)))
#define sign(n)((n) < 0 ? -1 : 1)

#endif
