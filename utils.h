#ifndef UTILS_H
#define UTILS_H

#include <SDL.h>
#include <SDL_ttf.h>

SDL_Texture *loadTexture(const char _file[], SDL_Renderer *ren);
void renderTexture2i(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture4i(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);
void renderTextureClip2i(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect *clip, int x, int y);

void renderText2i(TTF_Font *font, const char *str, SDL_Color color, SDL_Renderer *ren, int x, int y);

#endif // UNITLS_H

