#include <iostream>

#include "tod_core/proc_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_tod.h"
#include "tod_core/proc_tod_impl.h"
#include "tod_core/poli_tod.h"
#include "tod_core/inc_sdl.h"

#include "tod_py/tod_context.h"

#include "tod_py/inc_nanobind.h"
#include "tod_py/inc_nanobind_stl.h"
namespace nb = nanobind;

void nb_module(nanobind::module_ &m)
{
    // Initial set-up; similar to how the native demo works
    m.def("create_tod_context", [](){
        // Create the context as a unique ptr
        // We just set the w/h to fixed values
        std::unique_ptr<tod_py::tod_context> data = std::make_unique<tod_py::tod_context>();
        data->gpu_tex_w = 640;
        data->gpu_tex_h = 640;
        
        // Add gpu context
        tod::proc_gpu proc_gpu{};
        proc_gpu.create_gpu_context(data->gpu_context, { .create_window = false });
        
        // Add tod context
        tod::proc_tod proc_tod{};
        proc_tod.create_tod_context(data->tod_context, data->gpu_context, { .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM });

        // Create a texture which we render to
        SDL_GPUTextureCreateInfo gpu_tex_info{
		  .type                 = SDL_GPU_TEXTURETYPE_2D,
		  .format               = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		  .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
		  .width                = data->gpu_tex_w,
		  .height               = data->gpu_tex_h,
		  .layer_count_or_depth = 1,
		  .num_levels           = 1,
          .sample_count         = SDL_GPU_SAMPLECOUNT_1
	    };
	    data->gpu_tex = SDL_CreateGPUTexture(data->gpu_context.device, &gpu_tex_info);
        if (!data->gpu_tex) {
            std::cout << "ERROR: could not create gpu texture" << std::endl;
        }
            
        // Create a transfer buffer
        SDL_GPUTransferBufferCreateInfo gpu_transfer_buffer_info{
	      .usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD,
		  .size  = data->gpu_tex_w * data->gpu_tex_h * 4
        };
        data->gpu_transfer_buffer = SDL_CreateGPUTransferBuffer(data->gpu_context.device, &gpu_transfer_buffer_info);
        if (!data->gpu_transfer_buffer) {
            std::cout << "ERROR: could not create gpu transfer buffer" << std::endl;
        }

        return data;
    });

    // Clear the render texture
    m.def("clear", [](tod_py::tod_context& data, Eigen::Vector3f colour) { 
        tod::proc_tod t{};
        t.submit_pass_clear_texture(data.gpu_context, *data.gpu_tex, SDL_FColor{colour.x(), colour.y(), colour.z(), 1.f});
    });

    // Render the triangle to the texture
    m.def("render_triangle", [](tod_py::tod_context& data, const tod::proc_tod::render_triange_instr& instr) { 
        tod::proc_tod t{};
        t.submit_pass_render_triangle_to_texture(
          data.gpu_context, data.tod_context, instr, *data.gpu_tex
        );
    });

    // Copy the render texture to the CPU side and copy to a nanobind format
    m.def("get_image", [](tod_py::tod_context& data) {
        // Acquire GPU command buffer
        SDL_GPUCommandBuffer *cmd_buf = SDL_AcquireGPUCommandBuffer(data.gpu_context.device);
        
        // Create an transfer with region
        SDL_GPUTextureTransferInfo transfer_info{
	      .transfer_buffer = data.gpu_transfer_buffer,
          .offset          = 0
		};
        SDL_GPUTextureRegion region{
		  .texture   = data.gpu_tex,
          .mip_level = 0,
          .layer     = 0,
		  .w         = data.gpu_tex_w,
		  .h         = data.gpu_tex_h,
		  .d         = 1
        };

        // Add the download pass
	    SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd_buf);
	    SDL_DownloadFromGPUTexture(
		  copy_pass, &region, &transfer_info
	    );
	    SDL_EndGPUCopyPass(copy_pass);

        // Wait fro the fence (stalls the CPU)
        SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd_buf);
	    SDL_WaitForGPUFences(data.gpu_context.device, true, &fence, 1);
	    SDL_ReleaseGPUFence( data.gpu_context.device, fence);

        // Copy from the transfer buffer to a wholly new byte array
	    std::byte* transfer_ptr = (std::byte*)SDL_MapGPUTransferBuffer(
		    data.gpu_context.device, data.gpu_transfer_buffer, false
	    );
        std::byte* array_ptr = new std::byte[data.gpu_tex_w * data.gpu_tex_h * 4];
        nb::capsule owner(array_ptr, [](void *p) noexcept {
            delete[] (std::byte*)(p);
        });
        std::memcpy(array_ptr, transfer_ptr, data.gpu_tex_w * data.gpu_tex_h * 4);
	    SDL_UnmapGPUTransferBuffer(data.gpu_context.device, data.gpu_transfer_buffer);
        
        // Return as a numpy array, transferring ownership to the Python side
        return nb::ndarray<nb::numpy, std::uint8_t>(array_ptr, { data.gpu_tex_h, data.gpu_tex_w, 4 }, owner);
    });
}
