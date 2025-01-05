#pragma once

#include <array>
#include <string>
#include <fstream>
#include <type_traits>
#include "type_trait.h"

namespace tod::sugar {

	// Using 'keep' can let an rvalue be passed as an lvalue
	// Say there is a function
	//   void func(my_data& data)
	// then the invocation
	//   func(my_data{ .some_member = 1 })
	// will fail, as my_data is an rvalue. Using keep, this becomes
	//   func(keep(my_data{ .some_member = 1 })
	// and is allowed, as an lvalue to the struct is now passed
    template <class T>
    constexpr auto& keep(T &&x) noexcept {
        return x;
    }

    // Turn an arbitrary number of elements into an array
    // make_array(1, 2, 3, 4, 5) -> std::array<int, 5>
    template <class element_t, typename... Ts>
    constexpr auto make_array(element_t &&primary, Ts &&... runon) noexcept {
        return std::array<element_t, sizeof...(Ts) + 1>{ primary, runon... };
    }

    namespace detail {
        
        // Recursive variadic function which writes the data in a series of
        // containers to an output buffer comprised of individual elements
        template <typename element_t, typename container_t, int container_c, typename... Ts>
        constexpr auto recursive_write_concat_to_array(element_t *write, const container_t &primary, Ts const& ... runon) noexcept {
            const element_t *read  = (element_t*)(&primary);

            for (std::size_t i = 0; i < container_c; ++i)
              *(write++) = *(read++);
            
            if constexpr (sizeof...(runon) > 0)
                recursive_write_concat_to_array<element_t, container_t, container_c>(write, runon...);
        }
    }
    
    // Turn one or more containers into a single array of elements
    // For instance,
    //   some_matrix_4x4 my_data_0 = ...
    //   some_matrix_4x4 my_data_1 = ...
    //   concat_to_array(my_data_0, my_data_1) -> std::array<float, 32>
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
