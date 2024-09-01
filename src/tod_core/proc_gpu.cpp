#include "tod_core/proc_gpu.h"
#include "tod_core/data_gpu.h"
#include "tod_core/poli_gpu.h"

#include <iostream>

#include "tod_core/inc_sdl.h"
#include "tod_core/inc_shadercross.h"

using namespace tod;

proc_gpu::create_gpu_context_result proc_gpu::create_gpu_context(data_gpu_context& out, const poli_gpu_context& policy) const
{
    out.device = SDL_CreateGPUDevice(SDL_ShaderCross_GetShaderFormats(), policy.debug_mode, policy.name.size() > 0 ? policy.name.c_str() : nullptr);
    if (out.device == nullptr) {
        if (verbose_logging)
            std::cout << "ERROR: Failed to create GPU Device" << std::endl;
        return create_gpu_context_result::failure;
    }
    if (verbose_logging)
    {
        auto driver = SDL_GetGPUDriver(out.device);
        std::cout << "Created GPU Device with backend " << to_string(driver) << std::endl;
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

const char* proc_gpu::to_string(SDL_GPUDriver driver) const
{
    switch (driver)
    {
      case SDL_GPU_DRIVER_INVALID: return "invalid";
      case SDL_GPU_DRIVER_SECRET:  return "secret";
      case SDL_GPU_DRIVER_VULKAN:  return "vulkan";
      case SDL_GPU_DRIVER_D3D11:   return "dx11";
      case SDL_GPU_DRIVER_D3D12:   return "dx12";
      case SDL_GPU_DRIVER_METAL:   return "metal";
    }

    return "invalid";
}