#pragma once

#include "inc_eigen.h"

namespace tod::math {


    template<typename eigen_t, bool inner_panel>
    void look_at(Eigen::Block<eigen_t, 3, 4, inner_panel> out, Eigen::Vector3f const &pos, Eigen::Vector3f const &target, Eigen::Vector3f const &up)
    {
        out.col(2) = (pos - target).normalized();
        out.col(0) = up.cross(out.col(2)).normalized();
        out.col(1) = out.col(2).cross(out.col(0));
        out.topLeftCorner<3, 3>().transposeInPlace();
        out.col(3) = out.topLeftCorner<3, 3>() * -pos;
    }

    void perspective(Eigen::Matrix4f &out, float fov_y, float aspect, float near, float far)
    {
        float const θ       = fov_y * .5f;
        float const range   = far - near;
        float const inv_tan = 1.f / std::tan(θ);

        out <<
          inv_tan / aspect,     0.f,                 0.f,  0.f,
                       0.f, inv_tan,                 0.f,  0.f,
                       0.f,     0.f,        -far / range, -1.f,
                       0.f,     0.f, -far * near / range,  0.f
        ;
    }

}
