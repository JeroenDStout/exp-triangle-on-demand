#pragma once

/*
 *  ...
 */
 
#include "tod_core/data_gpu.h"
struct SDL_GPUTexture;
struct SDL_GPUTransferBuffer;

namespace tod_py {

    struct data_tod_py {
        
        tod::data_gpu_context context;
        SDL_GPUTexture        *gpu_tex;
        SDL_GPUTransferBuffer *gpu_transfer_buffer;

    };

}