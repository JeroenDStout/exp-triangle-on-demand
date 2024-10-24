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

    template <typename T, typename... Ts>
    constexpr auto make_array(const Ts&&... values) noexcept {
        using array_type = T;
        static_assert(sizeof...(Ts) > 0, "Array must have at least one element");
        return std::array<array_type, sizeof...(Ts)>{ static_cast<T>(values)... };
    }

}
