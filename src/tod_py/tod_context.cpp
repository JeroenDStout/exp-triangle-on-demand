#include "tod_py/tod_context.h"

#include "tod_core/proc_gpu.h"

#include "tod_py/inc_nanobind.h"
#include "tod_py/inc_nanobind_stl.h"
namespace nb = nanobind;

void nb_data_tod_py(nanobind::module_ &m)
{
    nb::class_<tod_py::tod_context>(m, "tod_context")
      .def("get_device_info", [](tod_py::tod_context& data) -> std::string {
        if (!data.gpu_context.device)
          return "No device";

        tod::proc_gpu p{};
        return p.create_debug_string(data.gpu_context);
      })
    ;
}
