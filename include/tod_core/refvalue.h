#pragma once

#include <array>
#include <string>
#include <fstream>
#include <type_traits>
#include "type_trait.h"

namespace tod::sugar {

    template <class T>
    constexpr auto& keep(T &&x) noexcept {
        return x;
    }

    template <class element_t, typename... Ts>
    constexpr auto make_array(element_t &&primary, Ts &&... runon) noexcept {
        return std::array<element_t, sizeof...(Ts) + 1>{ primary, runon... };
    }

    namespace detail {
        
        template <typename element_t, typename container_t, int container_c, typename... Ts>
        constexpr auto recursive_write_concat_to_array(element_t *write, const container_t &primary, Ts const& ... runon) noexcept {
            const element_t *read  = (element_t*)(&primary);

            for (std::size_t i = 0; i < container_c; ++i)
              *(write++) = *(read++);
            
            if constexpr (sizeof...(runon) > 0)
                recursive_write_concat_to_array<element_t, container_t, container_c>(write, runon...);
        }
    }
    
    template <class container_t, typename... Ts>
    constexpr auto concat_to_array(const container_t& primary, Ts const& ... runon) noexcept {
        using element_t   = tod::vector_element_type_v<container_t>;

        constexpr auto container_c = sizeof(container_t) / sizeof(element_t);
        constexpr auto element_c   = container_c * (sizeof...(runon) + 1);
        
        std::array<element_t, element_c> out;

        element_t *write = out.data();
        detail::recursive_write_concat_to_array<element_t, container_t, container_c>(write, primary, runon...);

        return out;
    }

}
