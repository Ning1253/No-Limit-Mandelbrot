#ifndef c_render_h
#define c_render_h

#include "decl.h"

void zoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y);
void unzoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y);
void shift(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double zoom_x, double zoom_y, direction dir);
void reset(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double* zoom_x, double* zoom_y);
void rdr_pnt(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], int x, int y);

#endif