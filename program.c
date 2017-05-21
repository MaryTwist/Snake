#include "program.h"

int program_init(program_t *p, int width, int height) {

  /* SDL initialization */
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    printf("SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  /* TTF initialization */
  if(TTF_Init()== -1) {
    printf("TTF_Init Error: %s\n", TTF_GetError());
    exit(1);
  }

  /* Create window */
#if FULLSCREEN == 1
  SDL_DisplayMode mode;
  SDL_GetDisplayMode(0, 0, &mode);
  p->win = SDL_CreateWindow("Hello World!", 100, 100, mode.w, mode.h, SDL_WINDOW_FULLSCREEN);
#else
  p->win = SDL_CreateWindow("Hello World!",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT,
                            SDL_WINDOW_SHOWN);
#endif
  if(NULL == p->win){
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  /* Create renderer */
  p->ren = SDL_CreateRenderer(p->win,
                              -1,
                              SDL_RENDERER_ACCELERATED/*SOFTWARE*/ | SDL_RENDERER_PRESENTVSYNC);
  if(NULL == p->ren){
    SDL_DestroyWindow(p->win);
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  /* Viewport */
#if FULLSCREEN == 1
  SDL_Rect vp;
  vp.x = mode.w / 2 - TILE_SIZE * MAP_WIDTH  / 2;
  vp.y = mode.h / 2 - TILE_SIZE * MAP_HEIGHT / 2;
  vp.w = TILE_SIZE * MAP_WIDTH;
  vp.h = TILE_SIZE * MAP_HEIGHT;
  SDL_RenderSetViewport(p->ren, &vp);
#endif

  /* resorces */
  program_load_resources(p);

  /* Program stuff */
  program_setup(p);

#if USE_GRAFFIC_BUFFER == 1
  /* grafics buffer */
  p->buffer = SDL_CreateTexture(p->ren,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET,
                                MAP_WIDTH * TILE_SIZE,
                                MAP_HEIGHT * TILE_SIZE);
#endif
}



int program_run(program_t *p) {
  int last_time = 0, current_time = 0, fps_counter = 0;

  while(!p->quit){

    current_time = SDL_GetTicks();
    fps_counter++;
    if(current_time - last_time > 1000) {
      last_time = current_time;
      p->fps = fps_counter;
      fps_counter = 0;
    }

    program_event_handler(p);
    program_update(p);
    program_render(p);

  }

  return 0;
}




int program_close(program_t *p) {

  /* Program stuff */
  program_cleanup(p);

  /* Release resources */
  program_free_resources(p);

#if USE_GRAFFIC_BUFFER == 1
  /* Release buffer */
  SDL_DestroyTexture(p->buffer);
#endif

  /* Release SDL */
  TTF_Quit();
  SDL_DestroyRenderer(p->ren);
  SDL_DestroyWindow(p->win);
  SDL_Quit();

  return 0;
}

inline void program_event_handler(program_t *p) {
  SDL_Event e;

  while(SDL_PollEvent(&e)){
    if(e.type == SDL_QUIT){
      p->quit = 1;
    }

    if(e.type == SDL_KEYDOWN){
      switch(e.key.keysym.sym){

        case SDLK_RIGHT:
          snake_setMove(p->hero, D_RIGHT);
          break;

        case SDLK_UP:
          snake_setMove(p->hero, D_UP);
          break;

        case SDLK_LEFT:
          snake_setMove(p->hero, D_LEFT);
          break;

        case SDLK_DOWN:
          snake_setMove(p->hero, D_DOWN);
          break;

        case SDLK_ESCAPE:
          p->quit = 1;
          break;

        default:
          break;
      }
    }

    if(e.type == SDL_MOUSEBUTTONDOWN){
      p->quit = 1;
    }
  }
}

inline void program_update(program_t *p)
{
  int snake_state;

  if((p->bonus->x == p->hero->body[0]->x)&&(p->bonus->y == p->hero->body[0]->y)) {
    snake_grow(p->hero);
    bonus_reset(p->bonus);
    p->score += 100;
  }

  bonus_update(p->bonus);

  snake_state = snake_update(p->hero);
#if SNAKE_RULE == SNAKE_RULE_FLAT
  if((snake_state == SNAKE_STATE_COLLIDE)||(snake_state == SNAKE_STATE_OUT)) {
#else
  if(snake_state == SNAKE_STATE_COLLIDE) {
#endif
    snake_reset(p->hero, 12, 12, D_RIGHT);
  };

}

inline void program_render(program_t *p)
{
  int str_size = 50;
  char str[str_size];

  /* clear */
  SDL_RenderClear(p->ren);

#if USE_GRAFFIC_BUFFER == 1
  /* set buffer */
  SDL_SetRenderTarget(p->ren, p->buffer);
#endif

  /* floor! */
  SDL_RenderCopy(p->ren, p->buffer_floor, NULL, &p->buffer_floor_rect);//&dst);

  /* hero!!! */
  snake_render(p->hero, p->ren);
  bonus_render(p->bonus, p->ren);

  /* important message */
  SDL_Color color = {35, 255, 255};

  snprintf(str, str_size, "FPS: %d", p->fps);
  renderText2i(p->font, str, color, p->ren, 16, 16);

  snprintf(str, str_size, "Size: %d", p->hero->size);
  renderText2i(p->font, str, color, p->ren, 16, 32);

  snprintf(str, str_size, "Score: %d", p->score);
  renderText2i(p->font, str, color, p->ren, 16, 56);

  /* draw directions of segments */
  /*
  for(int i = 0; i < p->hero->size; i++) {
    snprintf(str, str_size, "%d", p->hero->body[i]->d);
    renderText2i(p->font, str, color, p->ren,
      p->hero->body[i]->x * TILE_SIZE,
      p->hero->body[i]->y * TILE_SIZE);
  }
  */

  /* render! */
#if USE_GRAFFIC_BUFFER == 1
  SDL_SetRenderTarget(p->ren, NULL);
  renderTexture2i(p->buffer, p->ren, 0, 0);
  // Fit to screen
  //SDL_RenderCopy(p->ren, p->buffer, NULL, NULL);
#endif

  SDL_RenderPresent(p->ren);

}

inline void program_load_resources(program_t *p)
{
  p->font = TTF_OpenFont("gfx/Hardpixel.OTF", 16);
  if(!p->font) {
    printf("TTF_OpenFont Error: %s\n", TTF_GetError());
  }

  p->setting = loadTexture("gfx/snake_winter.bmp", p->ren);

  p->clip = malloc(sizeof(SDL_Rect) * CLIP_COUNT);
  for(int i = 0; i < CLIP_COUNT; i++) {
    p->clip[i] = malloc(sizeof(SDL_Rect));
  }

  /*
   *
   * SNAKE GFX
   *
   * */
  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_SNAKE_HEAD + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_SNAKE_HEAD + i]->y = 1;
    p->clip[CLIP_SNAKE_HEAD + i]->w = 24;
    p->clip[CLIP_SNAKE_HEAD + i]->h = 24;
  }

  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_SNAKE_BODY + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_SNAKE_BODY + i]->y = 26;
    p->clip[CLIP_SNAKE_BODY + i]->w = 24;
    p->clip[CLIP_SNAKE_BODY + i]->h = 24;
  }

  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_SNAKE_BEND + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_SNAKE_BEND + i]->y = 51;
    p->clip[CLIP_SNAKE_BEND + i]->w = 24;
    p->clip[CLIP_SNAKE_BEND + i]->h = 24;
  }

  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_SNAKE_TALE + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_SNAKE_TALE + i]->y = 76;
    p->clip[CLIP_SNAKE_TALE + i]->w = 24;
    p->clip[CLIP_SNAKE_TALE + i]->h = 24;
  }
  /*
   *
   * BONUS GFX
   *
   * */
  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_BONUS + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_BONUS + i]->y = 126;
    p->clip[CLIP_BONUS + i]->w = 24;
    p->clip[CLIP_BONUS + i]->h = 24;
  }

  /*
   *
   * FLOOR GFX
   *
   * */
  for(int i = 0; i < 4; i++) {
    p->clip[CLIP_FLOOR_1 + i]->x = (1 + i) + TILE_SIZE * i;
    p->clip[CLIP_FLOOR_1 + i]->y = 101;
    p->clip[CLIP_FLOOR_1 + i]->w = 24;
    p->clip[CLIP_FLOOR_1 + i]->h = 24;
  }

  /* make cash of floor image to speedup */
  p->buffer_floor_rect.x = 0;
  p->buffer_floor_rect.y = 0;
  p->buffer_floor_rect.w = MAP_WIDTH * TILE_SIZE;
  p->buffer_floor_rect.h = MAP_HEIGHT * TILE_SIZE;
  p->buffer_floor = SDL_CreateTexture(p->ren,
                                      SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      MAP_WIDTH * TILE_SIZE,
                                      MAP_HEIGHT * TILE_SIZE);

  /* floor */
  SDL_SetRenderTarget(p->ren, p->buffer_floor);
  for(int i = 0; i < MAP_HEIGHT; i++) { // rows
    for(int j = 0; j < MAP_WIDTH; j++) {
      renderTextureClip2i(p->setting, p->ren, p->clip[CLIP_FLOOR_1 + random(4)], j * TILE_SIZE, i * TILE_SIZE);
    }
  }
  SDL_SetRenderTarget(p->ren, NULL);
}

inline void program_free_resources(program_t *p)
{
  for(int i = 0; i < CLIP_COUNT; i++) {
    free(p->clip[i]);
  }
  free(p->clip);

  SDL_DestroyTexture(p->buffer_floor);
  SDL_DestroyTexture(p->setting);
}

inline void program_setup(program_t *p) {
  p->hero = snake_create(12, 12, D_RIGHT, p->setting, p->clip);
  p->bonus = bonus_create(8, 6, 10, 450, p->setting, p->clip);

  p->score = 0;
  p->score_best = 0;
}

inline void program_cleanup(program_t *p) {
  snake_free(p->hero);
  bonus_free(p->bonus);
}
