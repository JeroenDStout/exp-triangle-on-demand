#pragma once

#include <array>

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

namespace tod::data_shaders {

    struct unf_vert {
        std::array<float, 16> tr_object;
        std::array<float, 16> tr_camera_and_proj;
    };
    static_assert(sizeof(unf_vert) == 32 * 4);

    struct unf_frag {
        std::array<float, 12> colours;
    };
    static_assert(sizeof(unf_frag) == 12 * 4);

}
