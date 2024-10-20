#pragma once

/*
 *  ...
 */
 
#include <cstddef>
#include <cstdint>

namespace tod::vert {

    struct pos_col {
	    float        x, y, z;
        std::uint8_t r, g, b, a;
    };
    constexpr std::size_t pos_col_size = sizeof(pos_col);
    static_assert(pos_col_size == 16, "pos_col was padded");

}