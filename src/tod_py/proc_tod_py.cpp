#include <iostream>

#include "tod_core/proc_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_tod.h"
#include "tod_core/proc_tod_impl.h"
#include "tod_core/poli_tod.h"
#include "tod_core/inc_sdl.h"

#include "tod_py/inc_nanobind.h"
#include "tod_py/inc_nanobind_stl.h"
namespace nb = nanobind;

void nb_proc_tod_py(nanobind::module_ &m)
{
    using render_triange_instr = tod::proc_tod::render_triange_instr;
    using colour_t = std::array<float, 4>;

    nb::class_<render_triange_instr>(m, "render_triange_instr")
      .def(nb::init<>())
      .def_prop_rw("clear_colour",
        [](const render_triange_instr& ref)              { return *(colour_t*)(&ref.clear_colour); },
        [](render_triange_instr& ref, const colour_t &v) { *(colour_t*)(&ref.clear_colour) = v;    }
      )
      .def_prop_rw("triangle_size",
        [](const render_triange_instr& ref)              { return ref.triangle.size; },
        [](render_triange_instr& ref, float v)           { ref.triangle.size = v;    }
      )
      .def_prop_rw("triangle_spin",
        [](const render_triange_instr& ref)              { return ref.triangle.spin; },
        [](render_triange_instr& ref, float v)           { ref.triangle.spin = v;    }
      )
      .def_prop_rw("triangle_colour_1",
        [](const render_triange_instr& ref)              { return *(colour_t*)(&ref.triangle.colours[0]); },
        [](render_triange_instr& ref, const colour_t &v) { *(colour_t*)(&ref.triangle.colours[0]) = v;    }
      )
      .def_prop_rw("triangle_colour_2",
        [](const render_triange_instr& ref)              { return *(colour_t*)(&ref.triangle.colours[1]); },
        [](render_triange_instr& ref, const colour_t &v) { *(colour_t*)(&ref.triangle.colours[1]) = v;    }
      )
      .def_prop_rw("triangle_colour_3",
        [](const render_triange_instr& ref)              { return *(colour_t*)(&ref.triangle.colours[2]); },
        [](render_triange_instr& ref, const colour_t &v) { *(colour_t*)(&ref.triangle.colours[2]) = v;    }
      )
    ;
}
