#pragma once

#include "proc_tod.h"
#include "inc_eigen.h"
#include "inc_sdl.h"

struct tod::proc_tod::render_triange_instr {
    SDL_FColor clear_colour;

    struct triangle_ {
        float                     size    = 1.f;
        float                     spin    = 0.f;
        std::array<SDL_FColor, 3> colours = {{
          { 1.f, 0.f, 0.f, 1.f },
          { 0.f, 1.f, 0.f, 1.f },
          { 0.f, 0.f, 1.f, 1.f }
        }};
    } triangle;

    struct camera_ {
        Eigen::Vector3f           position = { 0.f, -10.f, 0.f };
        float                     fov_y    =    0.2f;
        float                     aspect   =    1.f;
    } camera;
};
