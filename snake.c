#include "snake.h"

Snake *snake_create(int x, int y, direction_t d, SDL_Texture *tex, SDL_Rect **clip) {
  Snake *o = malloc(sizeof(Snake));
  o->delay = 0;
  o->nd = D_RIGHT;
  o->tileset = tex;
  o->clip = clip;

  o->redraw = 1;
  o->buffer = NULL;
  // velocity
  o->v = SNAKE_VSTART;
  o->vmax = SNAKE_VMAX;

  // initialization of tale
  o->size = SNAKE_SIZE_INIT;
  o->body = malloc(sizeof(Body) * SNAKE_SIZE_MAX);

  for(int i = 0; i < SNAKE_SIZE_MAX; i++) {

    o->body[i] = malloc(sizeof(Body));
    o->body[i]->d = d;

    if(0 == i) {
      o->body[i]->x = x;
      o->body[i]->y = y;
      o->body[i]->px = x;
      o->body[i]->py = y;
    } else {
      o->body[i]->x = o->body[i]->px = o->body[0]->x - (1 + i);
      o->body[i]->y = o->body[i]->py = o->body[0]->y;
    }
  }

  // return new snake!
  return o;
}

void snake_reset(Snake *o, int x, int y, direction_t d) {
  o->size = SNAKE_SIZE_INIT;
  o->v = SNAKE_VSTART;
  o->nd = d;
  o->body[0]->x = x;
  o->body[0]->y = y;
  o->body[0]->d = d;

  for(int i = 0; i < o->size; i++) {
    o->body[i]->x = o->body[i]->px = x - (1 + i);
    o->body[i]->y = o->body[i]->py = y;
    o->body[i]->d = o->body[i]->pd = d;
  }
}

inline void snake_free(Snake *o) {
  if(NULL == o)
    return;

  for(int i = 0; i < SNAKE_SIZE_MAX; i++) {
    free(o->body[i]);
    printf("free(body[%d])\r\n", i);
  }

  free(o->body);
  free(o);
}

void snake_grow(Snake *o) {
  if(o->size >= SNAKE_SIZE_MAX) {
    return;
  }

  Body *part = o->body[o->size - 1];
  int dx, dy;
  dx = dy = 0;

  o->size++;

  switch(part->d) {
    case D_RIGHT:
      dx = -1;
      break;
    case D_UP:
      dy = 1;
      break;
    case D_LEFT:
      dx = 1;
      break;
    case D_DOWN:
      dy = -1;
      break;
  }

  o->body[o->size - 1]->d  = part->d;
  o->body[o->size - 1]->pd = part->pd;
  o->body[o->size - 1]->x  = part->x + dx;
  o->body[o->size - 1]->y  = part->y + dy;
}

inline void snake_setPos(Snake *o, int x, int y) {

}

inline void snake_setMove(Snake *o, direction_t d) {
  o->nd = d;
}

inline int snake_update(Snake *o) {
  int state = SNAKE_STATE_OK;

  /* delay */
  if(o->delay > 0) {
    o->delay--;
    return state;
  } else {
    o->delay = 0;
    o->redraw = 1;
  }

  /* movement */
  if(0 == o->delay) {

    // get next direction
    switch(o->nd) {
      case D_RIGHT:
        if(o->body[0]->d != D_LEFT) {
          o->body[0]->d = D_RIGHT;
        }
        break;

      case D_UP:
        if(o->body[0]->d != D_DOWN) {
          o->body[0]->d = D_UP;
        }
        break;

      case D_LEFT:
        if(o->body[0]->d != D_RIGHT) {
          o->body[0]->d = D_LEFT;
        }
        break;

      case D_DOWN:
        if(o->body[0]->d != D_UP) {
          o->body[0]->d = D_DOWN;
        }
        break;
    } // switch

    // save position
    o->body[0]->px = o->body[0]->x;
    o->body[0]->py = o->body[0]->y;
    o->body[0]->pd = o->body[0]->d;

    // do step
    switch(o->body[0]->d) {

      case D_RIGHT:
        o->body[0]->x += 1;
        break;

      case D_UP:
        o->body[0]->y -= 1;
        break;

      case D_LEFT:
        o->body[0]->x -= 1;
        break;

      case D_DOWN:
        o->body[0]->y += 1;
        break;

    }

    // torus rule
    if(o->body[0]->x >= MAP_WIDTH) { o->body[0]->x = 0; state = SNAKE_STATE_OUT; }
    if(o->body[0]->x < 0) { o->body[0]->x = MAP_WIDTH - 1; state = SNAKE_STATE_OUT; }
    if(o->body[0]->y >= MAP_HEIGHT) { o->body[0]->y = 0; state = SNAKE_STATE_OUT; }
    if(o->body[0]->y < 0) { o->body[0]->y = MAP_HEIGHT - 1; state = SNAKE_STATE_OUT; }

    // update tail and check if it states with snake`s head
    for(int i = 1; i < o->size; i++) {
      if(i >= SNAKE_SIZE_MAX) {
        break;
      }

      o->body[i]->px = o->body[i]->x;
      o->body[i]->py = o->body[i]->y;
      o->body[i]->pd = o->body[i]->d;

      o->body[i]->x = o->body[i-1]->px;
      o->body[i]->y = o->body[i-1]->py;
      o->body[i]->d = o->body[i-1]->pd;

      if((o->body[0]->x == o->body[i]->x)&&(o->body[0]->y == o->body[i]->y)) {
        state = SNAKE_STATE_COLLIDE;
      }
    }

    // reset delay
    o->delay = o->vmax - o->v;
  }

  return state;
}

inline void snake_render(Snake *o, SDL_Renderer *ren) {
/*
  if(1 == o->redraw) {

    if(NULL != o->buffer) {
      SDL_DestroyTexture(o->buffer);
      o->buffer = NULL;
    } // if

    o->buffer = SDL_CreateTexture(ren,
                                  SDL_PIXELFORMAT_RGBA8888,
                                  SDL_TEXTUREACCESS_TARGET,
                                  abs((o->x - o->body[o->size - 1]->x) + 1) * TILE_SIZE,
                                  abs((o->y - o->body[o->size - 1]->y) + 2) * TILE_SIZE);
    SDL_SetTextureBlendMode(o->buffer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(ren, o->buffer);
    SDL_SetRenderDrawColor(ren, 255, 0, 255, 0);//255);
    SDL_RenderClear(ren);
*/
    SDL_Rect dst;
    dst.w = TILE_SIZE; // o->clipHead->w;
    dst.h = TILE_SIZE; // TILE_SIZE; //o->clipHead->h;

    for(int i = 0; i < o->size; i++) {
      if(i >= SNAKE_SIZE_MAX) {
        break;
      } // if

      dst.x = o->body[i]->x * TILE_SIZE;
      dst.y = o->body[i]->y * TILE_SIZE;

      /* head */
      if(0 == i) {
        SDL_RenderCopy(ren, o->tileset, o->clip[CLIP_SNAKE_HEAD + o->body[i]->d], &dst);

      /* tale */
      } else if ((o->size - 1) == i){
        SDL_RenderCopy(ren, o->tileset, o->clip[CLIP_SNAKE_TALE + o->body[i]->d], &dst);

      /* body */
      } else {
        if(o->body[i]->d == o->body[i + 1]->d) {
          SDL_RenderCopy(ren, o->tileset, o->clip[CLIP_SNAKE_BODY + o->body[i]->d], &dst);
        } else {
          int new_d;
          Body *cur = o->body[i];
          Body *next = o->body[i + 1];

          if((cur->d == 2)&&(next->d == 3)) {
            new_d = 2;
          } else if((cur->d == 3)&&(next->d == 2)) {
            new_d = 1;
          } else if((cur->d == 1)&&(next->d == 0)) {
            new_d = 2;
          } else if((cur->d == 0)&&(next->d == 1)) {
            new_d = 1;
          } else if((cur->d == 1)&&(next->d == 2)) {
            new_d = 0;
          } else if((cur->d == 2)&&(next->d == 1)) {
            new_d = 3;
          } else if((cur->d == 3)&&(next->d == 0)) {
            new_d = 3;
          } else if((cur->d == 0)&&(next->d == 3)) {
            new_d = 0;
          } // if

          SDL_RenderCopy(ren, o->tileset, o->clip[CLIP_SNAKE_BEND + new_d], &dst);
        } // if
      } //if
    } // for
/*
    SDL_SetRenderTarget(ren, NULL);
    o->redraw = 0;
  } // if

  SDL_RenderCopy(ren, o->buffer, NULL, NULL);
*/
}
