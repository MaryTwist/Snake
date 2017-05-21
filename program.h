#ifndef PROGRAM_H
#define PROGRAM_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>

//#include <cutils.h>

#include "world.h"
#include "utils.h"
#include "snake.h"
#include "bonus.h"

#define program_create()(malloc(sizeof(program_t)))

typedef struct _program_t {

  /*
   *
   * SDL properties
   *
   * */
  SDL_Window *win;
  SDL_Renderer *ren;
  int quit;
  int fps;

  int score;
  int score_best;

  /*
   *
   * Program stuff
   *
   * */

  /* Gfx */
  TTF_Font *font;
  SDL_Texture *setting;
  SDL_Rect **clip;

#if USE_GRAFFIC_BUFFER == 1
  SDL_Texture *buffer;
#endif
  SDL_Rect buffer_floor_rect;
  SDL_Texture *buffer_floor;

  /* Objects */
  Snake *hero;
  Bonus *bonus;

} program_t;

/* program.c */
int program_init(program_t *, int, int);
int program_run(program_t *);
int program_close(program_t *);

void program_load_resources(program_t *);
void program_free_resources(program_t *);
void program_setup(program_t *);
void program_cleanup(program_t *);

void program_event_handler(program_t *);
void program_update(program_t *);
void program_render(program_t *);

#endif
