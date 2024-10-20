#pragma once

/*
 *  ...
 */
 
struct SDL_GPUDevice;
struct SDL_Window;

namespace tod {

    struct data_gpu_context {
        SDL_GPUDevice *device = nullptr;
        SDL_Window    *window = nullptr;
    };

}