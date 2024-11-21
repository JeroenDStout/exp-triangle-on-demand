#pragma once

#include <array>
#include <string>
#include <fstream>
#include <type_traits>

namespace tod::sugar {

    template <class T>
    constexpr auto& keep(T &&x) noexcept {
        return x;
    }

    template <typename array_t, typename... Ts>
    constexpr auto make_array(array_t &&primary, Ts &&... runon) noexcept {
        return std::array<array_t, sizeof...(Ts) + 1>{ primary, runon... };
    }

}
