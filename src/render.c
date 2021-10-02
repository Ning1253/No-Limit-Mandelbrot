#include "render.h"

void zoom(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], pxl copy[HEIGHT][WIDTH], double* zoom_x, double* zoom_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x % 2 == 0 && y % 2 == 0) {
                copy[y][x] = array[(HEIGHT / 2 + y) / 2][(WIDTH / 2 + x) / 2];

                if (copy[y][x].div) {
                    rdr_pnt(renderer, copy, x, y);
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
                copy[y][x] = array[2 * y - HEIGHT / 2][2 * x - WIDTH / 2];
            
                if (copy[y][x].div) {
                    rdr_pnt(renderer, copy, x, y);
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
                array[y][x] = array[y][x + WIDTH / 2];

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
                array[y][x] = array[y - HEIGHT / 2][x];

                if (array[y][x].div) {
                    rdr_pnt(renderer, array, x, y);
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
                array[y][x] = array[y][x - WIDTH / 2];

                if (array[y][x].div) {
                    rdr_pnt(renderer, array, x, y);
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
                array[y][x] = array[y + HEIGHT / 2][x];

                if (array[y][x].div) {
                    rdr_pnt(renderer, array, x, y);
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

void rdr_pnt(SDL_Renderer* renderer, pxl array[HEIGHT][WIDTH], int x, int y) {
    SDL_SetRenderDrawColor(renderer, r(array[y][x].step), g(array[y][x].step), b(array[y][x].step), 255);
    SDL_RenderDrawPoint(renderer, x, y);
}