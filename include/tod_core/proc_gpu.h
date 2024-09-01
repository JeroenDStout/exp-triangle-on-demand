#pragma once

/*
 *  ...
 */
 
enum SDL_GPUDriver : int;

namespace tod {

    struct data_gpu_context;
    struct poli_gpu_context;

    struct proc_gpu {
        
        bool verbose_logging = true;

        enum class create_gpu_context_result { success, failure };
        create_gpu_context_result create_gpu_context(data_gpu_context& out, const poli_gpu_context&) const;

        void destroy_gpu_context(data_gpu_context& inout) const;

        const char* to_string(SDL_GPUDriver) const;
    };

}