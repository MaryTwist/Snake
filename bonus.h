#ifndef BONUS_H
#define BONUS_H

#include <math.h>
#include <stdlib.h>
#include <SDL.h>
#include "utils.h"
#include "world.h"

typedef struct _bonus_t {
  int x;
  int y;

  int blink;
  int blink_timer;
  int blink_timer_delay;

  int reset_timer;
  int reset_timer_delay;

  int clip_id;
  SDL_Texture *tileset;
  SDL_Rect **clip;
} Bonus;

Bonus *bonus_create(int, int, int, int, SDL_Texture *, SDL_Rect **);
void bonus_free(Bonus *);
void bonus_update(Bonus *);
void bonus_render(Bonus *, SDL_Renderer *);
void bonus_reset(Bonus *);

#endif
