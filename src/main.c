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
void zoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y);
void unzoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y);
void shift(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double zoom_x, double zoom_y, direction dir);
void reset(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double* zoom_x, double* zoom_y);

int r(int x);
int g(int x);
int b(int x);


int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialise the SDL2 library. \n"); 
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("No Limit Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        printf("Failed to create window. \n");
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Failed to create the renderer from the window. \n");
        printf("%s\n", SDL_GetError());
        return -1;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    static pxl pixel_array[HEIGHT][WIDTH];
    static pxl array_back[HEIGHT][WIDTH];

    double zoom_x = SCALE_X;
    double zoom_y = SCALE_Y;
    double centre_x = 0, centre_y = 0;
    for (;;) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (!pixel_array[y][x].div) {
                    pixel_array[y][x].num = f(pixel_array[y][x].num, (centre_x + zoom_x * ((double)x / WIDTH - 0.5)) +   
                                                                     (centre_y + zoom_y * ((double)y / HEIGHT - 0.5)) * I);
                    pixel_array[y][x].step += 1;
                

                    if (cabsf(pixel_array[y][x].num) > 2) {
                        SDL_SetRenderDrawColor(renderer, r(pixel_array[y][x].step), g(pixel_array[y][x].step), b(pixel_array[y][x].step), 255);
                        SDL_RenderDrawPoint(renderer, x, y);
                        pixel_array[y][x].div = true;
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);

        SDL_Event event;
        while (SDL_PollEvent(&event) > 0) {
            if (event.type == SDL_QUIT) {
                SDL_Quit();
                return 0;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_EQUALS:
                        zoom(renderer, pixel_array, array_back, &zoom_x, &zoom_y);
                        break;
                    case SDL_SCANCODE_MINUS:
                        unzoom(renderer, pixel_array, array_back, &zoom_x, &zoom_y);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        shift(renderer, pixel_array, &centre_x, &centre_y, zoom_x, zoom_y, RIGHT);
                        break;
                    case SDL_SCANCODE_UP:
                        shift(renderer, pixel_array, &centre_x, &centre_y, zoom_x, zoom_y, UP);
                        break;
                    case SDL_SCANCODE_LEFT:
                        shift(renderer, pixel_array, &centre_x, &centre_y, zoom_x, zoom_y, LEFT);
                        break;
                    case SDL_SCANCODE_DOWN:
                        shift(renderer, pixel_array, &centre_x, &centre_y, zoom_x, zoom_y, DOWN);
                        break;
                    case SDL_SCANCODE_R:
                        reset(renderer, pixel_array, &centre_x, &centre_y, &zoom_x, &zoom_y);
                        break;
                }
                
            }
        }
    }
}


double complex f(double complex z, double complex c) {
    return z * z + c;
}

void zoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x % 2 == 0 && y % 2 == 0) {
                copy[y][x].div = array[(HEIGHT / 2 + y) / 2][(WIDTH / 2 + x) / 2].div;
                copy[y][x].num = array[(HEIGHT / 2 + y) / 2][(WIDTH / 2 + x) / 2].num;
                copy[y][x].step = array[(HEIGHT / 2 + y) / 2][(WIDTH / 2 + x) / 2].step;

                if (copy[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(copy[y][x].step), g(copy[y][x].step), b(copy[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            else {
                copy[y][x] = (pxl){0, 0, 0};
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            array[y][x] = copy[y][x];
        }
    }

    *zoom_x /= 2;
    *zoom_y /= 2;
}

void unzoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y >= HEIGHT / 4 && y < 3 * HEIGHT / 4 && x >= WIDTH / 4 && x < 3 * WIDTH / 4) {
                copy[y][x].div = array[2 * y - HEIGHT / 2][2 * x - WIDTH / 2].div;
                copy[y][x].num = array[2 * y - HEIGHT / 2][2 * x - WIDTH / 2].num;
                copy[y][x].step = array[2 * y - HEIGHT / 2][2 * x - WIDTH / 2].step;
            
                if (copy[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(copy[y][x].step), g(copy[y][x].step), b(copy[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }
            else {
                copy[y][x] = (pxl){0, 0, 0};
            }
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            array[y][x] = copy[y][x];
        }
    }

    *zoom_x *= 2;
    *zoom_y *= 2;
}

void shift(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double zoom_x, double zoom_y, direction dir) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (dir == RIGHT) {
        *centre_x += zoom_x / 2;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH / 2; x++) {
                array[y][x].div = array[y][x + WIDTH / 2].div;
                array[y][x].num = array[y][x + WIDTH / 2].num;
                array[y][x].step = array[y][x + WIDTH / 2].step;

                if (array[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(array[y][x].step), g(array[y][x].step), b(array[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }

            for (int x = WIDTH / 2; x < WIDTH; x++) {
                array[y][x] = (pxl){0, 0, 0};
            }
        }
    }

    else if (dir == UP) {
        *centre_y -= zoom_y / 2;

        for (int x = 0; x < WIDTH; x++) {
            for (int y = HEIGHT / 2; y < HEIGHT; y++) {
                array[y][x].div = array[y - HEIGHT / 2][x].div;
                array[y][x].num = array[y - HEIGHT / 2][x].num;
                array[y][x].step = array[y - HEIGHT / 2][x].step;

                if (array[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(array[y][x].step), g(array[y][x].step), b(array[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }

            for (int y = 0; y < HEIGHT / 2; y++) {
                array[y][x] = (pxl){0, 0, 0};
            }
        }
    }

    else if (dir == LEFT) {
        *centre_x -= zoom_x / 2;

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = WIDTH / 2; x < WIDTH; x++) {
                array[y][x].div = array[y][x - WIDTH / 2].div;
                array[y][x].num = array[y][x - WIDTH / 2].num;
                array[y][x].step = array[y][x - WIDTH / 2].step;

                if (array[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(array[y][x].step), g(array[y][x].step), b(array[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }

            for (int x = 0; x < WIDTH / 2; x++) {
                array[y][x] = (pxl){0, 0, 0};
            }
        }
    }

    else if (dir == DOWN) {
        *centre_y += zoom_y / 2;

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT / 2; y++) {
                array[y][x].div = array[y + HEIGHT / 2][x].div;
                array[y][x].num = array[y + HEIGHT / 2][x].num;
                array[y][x].step = array[y + HEIGHT / 2][x].step;

                if (array[y][x].div) {
                    SDL_SetRenderDrawColor(renderer, r(array[y][x].step), g(array[y][x].step), b(array[y][x].step), 255);
                    SDL_RenderDrawPoint(renderer, x, y);
                }
            }

            for (int y = HEIGHT / 2; y < HEIGHT; y++) {
                array[y][x] = (pxl){0, 0, 0};
            }
        }
    }
}

void reset(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], double* centre_x, double* centre_y, double* zoom_x, double* zoom_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            array[y][x] = (pxl){0, 0, 0};
        }
    }

    *zoom_x = SCALE_X;
    *zoom_y = SCALE_Y;
    *centre_x = 0;
    *centre_y = 0;

}
/*
int r(int x) {
    return (int)(255 - (abs(128 - x) % 128));
}

int g(int x) {
   return (int)(206 - (abs(66 - x) % 66));
}

int b(int x) {
    return (int)(0 + (abs(225 - x) % 225));
}
*/
//COLOUR GRADUAL ABS
int r(int x) {
    return abs(128 - x % 256);
}

int g(int x) {
   return abs(128 - x % 256);
}

int b(int x) {
    return abs(128 - x % 256);
}


/* BLACK WHITE SINE LINEAR
int r(int x) {
    return (int)(256 * sin(x)) % 256;
}

int g(int x) {
   return (int)(256 * sin(x)) % 256;
}

int b(int x) {
    return (int)(256 * sin(x)) % 256;
}
*/