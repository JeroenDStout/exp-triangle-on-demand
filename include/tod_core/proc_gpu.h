#pragma once

/*
 *  ...
 */

 #include <string>
 
enum SDL_GPUDriver      : int;
enum SDL_GPUShaderStage : int;
struct SDL_GPUShader;

namespace tod {

    struct data_gpu_context;
    struct poli_gpu_context;
    struct poli_gpu_shader;

    struct proc_gpu {
        
        bool verbose_logging = true;

        enum class create_gpu_context_result { success, failure };
        create_gpu_context_result create_gpu_context(data_gpu_context &out, const poli_gpu_context&) const;
        
        struct create_gpu_shader_instr {
            SDL_GPUShaderStage  stage{};
	        std::string_view    file_data{};
	        std::uint32_t       count_sampler         = 0;
	        std::uint32_t       count_buffer_uniform  = 0;
	        std::uint32_t       count_buffer_storage  = 0;
	        std::uint32_t       count_texture_storage = 0;
        };
        enum class create_gpu_shader_result { success, failure };
        create_gpu_shader_result create_gpu_shader(SDL_GPUShader *&out, data_gpu_context &gpu_context, const create_gpu_shader_instr &instr, const poli_gpu_shader&) const;

        void destroy_gpu_context(data_gpu_context& inout) const;
        
        std::string create_debug_string(data_gpu_context& context) const;

      protected:
        const char* to_string(SDL_GPUDriver) const;
    };

}