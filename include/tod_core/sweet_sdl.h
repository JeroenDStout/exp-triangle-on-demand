#pragma once
#include "type_trait.h"
#include "inc_sdl.h"

namespace tod {

    // Helper for explicit fcolor element type
    template<>
    struct vector_element_type<SDL_FColor> {
      using value = float;
    };

}