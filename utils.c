#include "utils.h"

SDL_Texture *loadTexture(const char _file[], SDL_Renderer *ren){
  SDL_Texture *tex;
  SDL_Surface *img;

  tex = NULL;
  img = SDL_LoadBMP(_file);
  SDL_SetColorKey(img, SDL_TRUE, 0xff00ff);
  if(NULL != img){
    tex =  SDL_CreateTextureFromSurface(ren, img);
    SDL_FreeSurface(img);
  }

  return tex;
}

void renderTexture2i(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;

  SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture4i(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = w;
  dst.h = h;
  SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTextureClip2i(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect *clip, int x, int y){

  if(NULL == clip){
    return;
  }

  SDL_Rect dst;
  dst.x = x;
  dst.y = y;
  dst.w = clip->w;
  dst.h = clip->h;

  SDL_RenderCopy(ren, tex, clip, &dst);
}

inline void renderText2i(TTF_Font *font, const char *str, SDL_Color color,
                  SDL_Renderer *ren, int x, int y)
{
  // destination
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;

  // surface and texture
  SDL_Surface *sur = TTF_RenderUTF8_Solid(font, str, color);
  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, sur);

  // rendering
  SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
  SDL_RenderCopy(ren, tex, NULL, &dest);

  // release surface and texture
  SDL_FreeSurface(sur);
  SDL_DestroyTexture(tex);
}

