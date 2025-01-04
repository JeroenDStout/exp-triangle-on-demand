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

    nb::class_<render_triange_instr>(m, "render_triange_instr")
      .def(nb::init<>())
      .def_rw("triangle_spin", &render_triange_instr::triangle_spin)
    ;
}
