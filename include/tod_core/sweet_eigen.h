#pragma once

#include <array>
#include <string>
#include <fstream>
#include <type_traits>

namespace tod::sugar {

    template <typename eigen_t>
    constexpr auto eig_as_array(eigen_t &eig) noexcept {
        using scalar_t = eigen_t::Scalar;
        using array_t  = std::array<scalar_t, eigen_t::SizeAtCompileTime>;
        return *(array_t*)(&eig);
    }

}
