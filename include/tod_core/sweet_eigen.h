#pragma once

#include <array>

namespace tod::sugar {

    // Reinterpret an eigen matrix as an std::array
    // Obvious terms and conditions apply for eigen types
    template <typename eigen_t>
    constexpr auto eig_as_array(eigen_t &eig) noexcept {
        using scalar_t = eigen_t::Scalar;
        using array_t  = std::array<scalar_t, eigen_t::SizeAtCompileTime>;
        return *(array_t*)(&eig);
    }

}
