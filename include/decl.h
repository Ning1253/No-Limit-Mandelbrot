#ifndef c_decs_h
#define c_decs_h

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <complex.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>


#define WIDTH 1920
#define HEIGHT 1080

#define SCALE_X 8
#define SCALE_Y 4.5


typedef struct {
    double complex num;
    int step;
    bool div;
} pxl;

typedef enum {
    RIGHT, 
    UP, 
    LEFT, 
    DOWN
} direction;

double complex f(double complex z, double complex c);

int r(int x);
int g(int x);
int b(int x);

#endif