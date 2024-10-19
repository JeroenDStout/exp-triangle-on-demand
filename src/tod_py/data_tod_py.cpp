#include "tod_py/data_tod_py.h"

#include <iostream>

#include "tod_core/proc_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_tod.h"
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
		  .type                 = SDL_GPU_TEXTURETYPE_2D,
		  .format               = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		  .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		  .width                = 640,
		  .height               = 480,
		  .layer_count_or_depth = 1,
		  .num_levels           = 1,
          .sample_count         = SDL_GPU_SAMPLECOUNT_1
	    };
	    data->gpu_tex = SDL_CreateGPUTexture(data->context.device, &gpu_tex_info);
        if (!data->gpu_tex) {
            std::cout << "ERROR: could not create gpu texture" << std::endl;
        }
            
        SDL_GPUTransferBufferCreateInfo gpu_transfer_buffer_info{
	      .usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD,
		  .size  = 640 * 480 * 4
        };
        data->gpu_transfer_buffer = SDL_CreateGPUTransferBuffer(data->context.device, &gpu_transfer_buffer_info);
        if (!data->gpu_transfer_buffer) {
            std::cout << "ERROR: could not create gpu transfer buffer" << std::endl;
        }

        return data;
    });

    m.def("clear", [](tod_py::data_tod_py& data, [[maybe_unused]] Eigen::Vector3f colour) { 
        tod::proc_tod t{};
        t.submit_pass_clear_texture(data.context, *data.gpu_tex, SDL_FColor{colour.x(), colour.y(), colour.z(), 1.f});
    });

    m.def("get_image", [](tod_py::data_tod_py& data) {
        SDL_GPUCommandBuffer *cmd_buf = SDL_AcquireGPUCommandBuffer(data.context.device);
        
        SDL_GPUTextureTransferInfo transfer_info{
	      .transfer_buffer = data.gpu_transfer_buffer,
          .offset          = 0
		};
        SDL_GPUTextureRegion region{
		  .texture   = data.gpu_tex,
          .mip_level = 0,
          .layer     = 0,
		  .w         = 640,
		  .h         = 480,
		  .d         = 1
        };

	    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd_buf);
	    SDL_DownloadFromGPUTexture(
		  copy_pass, &region, &transfer_info
	    );
	    SDL_EndGPUCopyPass(copy_pass);

        SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd_buf);
	    SDL_WaitForGPUFences(data.context.device, true, &fence, 1);
	    SDL_ReleaseGPUFence( data.context.device, fence);

	    std::byte* transfer_ptr = (std::byte*)SDL_MapGPUTransferBuffer(
		    data.context.device, data.gpu_transfer_buffer, false
	    );
        std::byte* array_ptr = new std::byte[640 * 480 * 4];
        nb::capsule owner(array_ptr, [](void *p) noexcept {
            delete[] (std::byte*)(p);
        });
        std::memcpy(array_ptr, transfer_ptr, 640 * 480 * 4);

	    SDL_UnmapGPUTransferBuffer(data.context.device, data.gpu_transfer_buffer);
        
        return nb::ndarray<nb::numpy, std::uint8_t>(array_ptr, { 640, 480, 4 }, owner);
    });
}
