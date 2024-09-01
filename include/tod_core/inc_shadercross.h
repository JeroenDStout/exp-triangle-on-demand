#pragma once

#include "inc_sdl.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4191) // unsafe conversion
#  pragma warning(disable : 4701) // potentially uninitialized local variable
#endif

#include <shadercross/SDL_gpu_shadercross.h>

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
