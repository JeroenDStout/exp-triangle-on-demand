#pragma once

/*
 *  ...
 */

struct SDL_GPUShader;

namespace tod {

    struct data_tod_context {
        
        SDL_GPUShader   *vert_shader,
                        *frag_shader;

    };

}