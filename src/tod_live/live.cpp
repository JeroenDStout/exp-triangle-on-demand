#include "tod_live/live.h"

#include <iostream>
#include <random>

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
    
    tod::poli_tod_context tod_context_poli{};
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

    for (int i = 0; i < 10; ++i)
    {
        proc_tod.submit_pass_draw_triangle(gpu_context, tod_context, { dist(e2), dist(e2), dist(e2), 1.f });
        SDL_Delay(300);
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