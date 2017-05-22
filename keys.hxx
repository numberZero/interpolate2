#pragma once

#ifdef HAVE_SDL
#include <SDL.h>
#define KEY_1	SDL_SCANCODE_1
#define KEY_2	SDL_SCANCODE_2
#define KEY_3	SDL_SCANCODE_3
#define KEY_M	SDL_SCANCODE_M
#define KEY_R	SDL_SCANCODE_R
#define KEY_D	SDL_SCANCODE_D
#define KEY_E	SDL_SCANCODE_E
#define KEY_KP_PLUS	SDL_SCANCODE_KP_PLUS
#define KEY_KP_MINUS	SDL_SCANCODE_KP_MINUS
#define KEY_KP_MULTIPLY	SDL_SCANCODE_KP_MULTIPLY
#define KEY_KP_DIVIDE	SDL_SCANCODE_KP_DIVIDE
#define KEY_KP_4	SDL_SCANCODE_KP_4
#define KEY_KP_6	SDL_SCANCODE_KP_6
#define KEY_ESCAPE	SDL_SCANCODE_ESCAPE
#endif

#ifdef HAVE_X11
#define KEY_1	10
#define KEY_2	11
#define KEY_3	12
#define KEY_M	58
#define KEY_R	27
#define KEY_D	40
#define KEY_E	26
#define KEY_KP_PLUS	86
#define KEY_KP_MINUS	82
#define KEY_KP_MULTIPLY	63
#define KEY_KP_DIVIDE	106
#define KEY_KP_4	83
#define KEY_KP_6	85
#define KEY_ESCAPE	9
#endif