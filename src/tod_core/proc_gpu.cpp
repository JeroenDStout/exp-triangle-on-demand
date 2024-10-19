#include "tod_core/proc_gpu.h"
#include "tod_core/data_gpu.h"
#include "tod_core/poli_gpu.h"

#include <iostream>
#include <sstream>

#include "tod_core/inc_sdl.h"
#include "tod_core/inc_shadercross.h"

using namespace tod;

proc_gpu::create_gpu_context_result proc_gpu::create_gpu_context(data_gpu_context& out, const poli_gpu_context& policy) const
{
    out.device = SDL_CreateGPUDevice(SDL_ShaderCross_GetSPIRVShaderFormats(), policy.debug_mode, policy.name.size() > 0 ? policy.name.c_str() : nullptr);
    if (out.device == nullptr) {
        if (verbose_logging)
            std::cout << "ERROR: Failed to create GPU Device" << std::endl;
        return create_gpu_context_result::failure;
    }
    if (verbose_logging)
    {
        std::cout << "Created GPU Device with backend " << SDL_GetGPUDeviceDriver(out.device) << std::endl;
    }

    if (policy.create_window) {
        out.window = SDL_CreateWindow(policy.window_name.size() > 0 ? policy.window_name.c_str() : "SDL Window", policy.window_w, policy.window_h, 0x0);
        if (out.window == nullptr) {
            std::cout << "ERROR: CreateWindow failed: " << SDL_GetError() << std::endl;
            return create_gpu_context_result::failure;
        }

        if (!SDL_ClaimWindowForGPUDevice(out.device, out.window))
        {
            std::cout << "ERROR: ClaimWindowForGPUDevice failed" << std::endl;
            return create_gpu_context_result::failure;
        }
    }

    return create_gpu_context_result::success;
}

auto tod::proc_gpu::create_gpu_shader(SDL_GPUShader*& out, data_gpu_context& gpu_context, const create_gpu_shader_instr& instr, const poli_gpu_shader&) const -> create_gpu_shader_result
{
    SDL_GPUShaderCreateInfo info{
      .code_size            = instr.file_data.size(),
      .code                 = (std::uint8_t*)instr.file_data.data(),
      .entrypoint           = "main",
      .format               = SDL_GPU_SHADERFORMAT_SPIRV,
      .stage                = instr.stage,
      .num_samplers         = instr.count_sampler,
      .num_storage_textures = instr.count_texture_storage,
      .num_storage_buffers  = instr.count_buffer_storage,
      .num_uniform_buffers  = instr.count_buffer_uniform
    };
	out = (SDL_GPUShader*)SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(gpu_context.device, &info);
	
	if (out)
      return create_gpu_shader_result::success;

	std::cout << "Failed to create shader: " << SDL_GetError() << std::endl;
    return create_gpu_shader_result::failure;
}

void proc_gpu::destroy_gpu_context(data_gpu_context& inout) const
{
    if (inout.window)
    {
	    SDL_ReleaseWindowFromGPUDevice(inout.device, inout.window);
	    SDL_DestroyWindow(inout.window);
        inout.window = nullptr;
    }
    
	SDL_DestroyGPUDevice(inout.device);
    inout.device = nullptr;
}

std::string proc_gpu::create_debug_string(data_gpu_context& context) const
{
    std::stringstream ss;
    ss << "context {";
    ss << " gpu_driver: " << SDL_GetGPUDeviceDriver(context.device);
    ss << " }";

    return ss.str();
}
