#pragma once

/*
 *  ...
 */

struct SDL_GPUBuffer;
struct SDL_GPUShader;
struct SDL_GPUGraphicsPipeline;

namespace tod {

    struct data_tod_context {
        
        SDL_GPUShader           *vert_shader,
                                *frag_shader;
                                
        SDL_GPUBuffer           *vert_buffer;

        SDL_GPUGraphicsPipeline *pipeline;

    };

}