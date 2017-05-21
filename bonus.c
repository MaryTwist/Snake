#include "bonus.h"

Bonus *bonus_create(int x, int y, int blink_delay, int reset_delay, SDL_Texture *tileset, SDL_Rect **clip) {
  Bonus *o = malloc(sizeof(Bonus));
  o->x = x;
  o->y = y;
  o->tileset = tileset;
  o->clip = clip;
  o->blink_timer = o->blink_timer_delay = blink_delay;
  o->reset_timer = o->reset_timer_delay = reset_delay;
  o->clip_id = random(4);

  return o;
}

void bonus_free(Bonus *o) {
  if(NULL == o)
    return;

  free(o);
}

inline void bonus_update(Bonus *o) {
  if(o->blink_timer > 0) {
    o->blink_timer--;
  } else {
    o->blink_timer = o->blink_timer_delay;
    o->blink = !o->blink;
  }
/*
  if(o->reset_timer > 0) {
    o->reset_timer--;
  } else {
    o->reset_timer = o->reset_timer_delay;
    o->x = random(MAP_WIDTH);
    o->y = random(MAP_HEIGHT);
  }
*/
}

inline void bonus_render(Bonus *o, SDL_Renderer *ren) {
  SDL_Rect dst;

  if(!o->blink) {
    return;
  }

  dst.w = TILE_SIZE; // o->clipHead->w;
  dst.h = TILE_SIZE; // TILE_SIZE; //o->clipHead->h;
  dst.x = o->x * TILE_SIZE;
  dst.y = o->y * TILE_SIZE;
  SDL_RenderCopy(ren, o->tileset, o->clip[CLIP_BONUS + o->clip_id], &dst);
}

void bonus_reset(Bonus *o) {
  o->reset_timer = o->reset_timer_delay;
  o->x = random(MAP_WIDTH);
  o->y = random(MAP_HEIGHT);
  o->clip_id = random(4);
}
