// Test test test

#include <iostream>

#include "version/git_version.h"
#include "tod_core/inc_sdl.h"

int main(int argc, char* args[])
{
    SDL_Window* sdl_window;

    // Initialize SDL
    if (auto result = SDL_Init( SDL_INIT_VIDEO); result != SDL_TRUE)
    {
        std::cout << "SDL could not initialize! SDL_Error:" << SDL_GetError() << std::endl;
        return -1;
    }

    sdl_window = SDL_CreateWindow("SDL Example", 1024, 768, 0x0);
    if (!sdl_window)
    {
        std::cout << "SDL could not create a window! SDL_Error:" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Delay(3000);

    SDL_DestroyWindow(sdl_window);
    SDL_Quit();

    return 0;
}