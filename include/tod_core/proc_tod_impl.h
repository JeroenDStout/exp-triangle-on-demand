#pragma once

#include "proc_tod.h"
#include "inc_eigen.h"
#include "inc_sdl.h"

struct tod::proc_tod::render_triange_instr {
    SDL_FColor      clear_colour;
    Eigen::Matrix4f transform_object = Eigen::Matrix4f::Identity();
    float           camera_aspect = 1.f;
};
