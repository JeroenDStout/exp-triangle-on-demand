#include "tod_py/data_tod_py.h"

#include <iostream>

#include "tod_core/proc_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/inc_sdl.h"

#include "tod_py/inc_nanobind.h"
#include "tod_py/inc_nanobind_stl.h"
namespace nb = nanobind;

void nb_data_tod_py(nanobind::module_ &m)
{
    nb::class_<tod_py::data_tod_py>(m, "tod_context")
      .def("get_device_info", [](tod_py::data_tod_py& data) -> std::string {
        if (!data.context.device)
          return "No device";

        tod::proc_gpu p{};
        return p.create_debug_string(data.context);
      })
    ;

    m.def("create_tod_context", [](){ 
        std::unique_ptr<tod_py::data_tod_py> data = std::make_unique<tod_py::data_tod_py>();
        
        tod::proc_gpu p{};
        p.create_gpu_context(data->context, { .create_window = false });
        
        SDL_GPUTextureCreateInfo gpu_tex_info{
		  .type              = SDL_GPU_TEXTURETYPE_2D,
		  .format            = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		  .usageFlags        = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET_BIT | SDL_GPU_TEXTUREUSAGE_GRAPHICS_STORAGE_READ_BIT,
		  .width             = 640,
		  .height            = 480,
		  .layerCountOrDepth = 1,
		  .levelCount        = 1,
          .sampleCount       = SDL_GPU_SAMPLECOUNT_1
	    };
	    data->gpu_tex = SDL_CreateGPUTexture(data->context.device, &gpu_tex_info);
        if (!data->gpu_tex) {
            std::cout << "ERROR: could not create gpu texture" << std::endl;
        }

        return data;
    });
}