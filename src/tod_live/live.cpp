#include "tod_live/live.h"

#include <iostream>
#include <random>

#include "tod_core/proc_tod_impl.h"
#include "tod_core/inc_sdl.h"

using namespace tod_live;

auto handler_tod_live::prepare() -> handler_result
{
    proc_gpu = tod::proc_gpu{ .verbose_logging = true };
    proc_tod = tod::proc_tod{ .verbose_logging = true };

    gpu_context = tod::data_gpu_context{};
    tod_context = tod::data_tod_context{};

    return handler_result::success;
}

auto handler_tod_live::init() -> handler_result
{
    tod::poli_tod_init poli_tod{};
    if (!proc_tod.init_tod(poli_tod))
    {
        std::cout << "ERROR: Error init'ing ToD" << std::endl;
        return handler_result::failure;
    }

    tod::poli_gpu_context gpu_context_poli{};
    proc_gpu.create_gpu_context(this->gpu_context, gpu_context_poli);
    
    tod::poli_tod_context tod_context_poli{ .format = SDL_GetGPUSwapchainTextureFormat(this->gpu_context.device, this->gpu_context.window) };
    if (!proc_tod.create_tod_context(tod_context, gpu_context, tod_context_poli))
    {
        std::cout << "ERROR: Error creating ToD context" << std::endl;
        return handler_result::failure;
    }

    return handler_result::success;
}

auto handler_tod_live::run() -> handler_result
{
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    SDL_FColor current_clear_colour{};
    float      next_clear_colour_time = 0.f;
    float      next_clear_colour_step = 1.f;

    auto time_start = std::chrono::steady_clock::now();
    for (;;) {
        auto time = std::chrono::steady_clock::now();
        auto time_passed = std::chrono::duration_cast<std::chrono::nanoseconds>(time - time_start).count() / 1e9f;
        if (time_passed > 7.f)
          break;
        
        if (time_passed >= next_clear_colour_time)
        {
            current_clear_colour = { dist(e2), dist(e2), dist(e2), 1.f };
            next_clear_colour_time += next_clear_colour_step;
            next_clear_colour_step *= 0.85f;
        }
        
        proc_tod.submit_pass_render_triangle_to_window(gpu_context, tod_context, tod::proc_tod::render_triange_instr{
          .clear_colour  = current_clear_colour,
          .triangle = {
            .spin = time_passed * time_passed * 2.f
          }
        });
    }

    return handler_result::success;
}

auto handler_tod_live::deinit() -> handler_result
{
    proc_gpu.destroy_gpu_context(gpu_context);

    return handler_result::success;
}

auto handler_tod_live::cleanup() -> handler_result
{
    return handler_result::success;
}