// Test test test

#include <iostream>
#include <random>

#include "repo_version/git_version.h"

#include "tod_core/data_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_gpu.h"
#include "tod_core/proc_tod.h"
#include "tod_core/poli_tod.h"

#include "tod_core/inc_sdl.h"

int main(int, char*[])
{
    std::cout
      << std::endl
      << "          * * * * * * * ** * * * * * * " << std::endl
      << "         * * * Triangle on Demand * * * " << std::endl
      << "          * * * * * * * ** * * * * * * " << std::endl
      << std::endl
      << gaos::version::get_git_essential_version() << std::endl
      << gaos::version::get_compile_stamp() << std::endl
      << std::endl
      << gaos::version::get_git_history() << std::endl
      << std::endl;

    tod::proc_gpu proc_gpu{};
    tod::proc_tod proc_tod{};
    tod::poli_tod_init poli_tod{};

    if (!proc_tod.init_tod(poli_tod))
    {
        std::cout << "ERROR: Error init'ing ToD" << std::endl;
        return -1;
    }

    tod::poli_gpu_context poli_gpu_context{};
    tod::data_gpu_context gpu_context{};

    proc_gpu.create_gpu_context(gpu_context, poli_gpu_context);

    // Random clears
    {
        std::random_device rd;
        std::mt19937 e2(rd());
        std::uniform_real_distribution<float> dist(0.f, 1.f);

        for (int i = 0; i < 10; ++i)
        {
            proc_tod.submit_pass_clear_window(gpu_context, { dist(e2), dist(e2), dist(e2), 1.f });
            SDL_Delay(300);
        }
    }

    proc_gpu.destroy_gpu_context(gpu_context);

    return 0;
}
