#pragma once

#include <string>
#include <fstream>

namespace tod::sugar {

    template <class T>
    constexpr auto& keep(T&& x) noexcept {
        return x;
    }

}