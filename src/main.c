#include "decl.h"
#include "render.h"

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
                        rdr_pnt(renderer, pixel_array, x, y);
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