#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>

#include <stdlib.h>
#include <SDL.h>
#include "utils.h"
#include "world.h"

typedef enum _direction_t { D_RIGHT = 0, D_UP = 1, D_LEFT = 2, D_DOWN = 3 } direction_t;

typedef struct _body_t {
  int x;
  int y;
  int px;
  int py;
  direction_t d;
  direction_t pd;
} Body;

typedef struct _snake_t {

  direction_t nd;

  int v; // velosity
  int vmax; // max of velocity
  int delay; // step delay

  int size;
  Body **body;

  int redraw;
  SDL_Texture *buffer;
  SDL_Texture *tileset;
  SDL_Rect **clip;
} Snake;

Snake *snake_create(int, int, direction_t, SDL_Texture *, SDL_Rect **);
void snake_reset(Snake *, int, int, direction_t);
void snake_free(Snake *);
void snake_grow(Snake *);
void snake_setPos(Snake *, int, int);
void snake_setMove(Snake *, direction_t);
int  snake_update(Snake *);
void snake_render(Snake *, SDL_Renderer *);

#endif
