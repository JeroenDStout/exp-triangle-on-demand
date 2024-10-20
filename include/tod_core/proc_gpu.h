#pragma once

/*
 *  ...
 */

 #include <string>
 
enum SDL_GPUDriver      : int;
enum SDL_GPUShaderStage : int;
struct SDL_GPUBuffer;
struct SDL_GPUShader;

namespace tod {

    struct data_gpu_context;
    struct poli_gpu_context;
    struct poli_gpu_shader;

    struct proc_gpu {
        
        bool verbose_logging = true;

        using sdl_size_t = std::uint32_t;

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
        
        enum class perform_gpu_upload_pass_result { success, failure };
        struct data_gpu_upload_pass_context;
        template<typename l_fill_data_t>
        perform_gpu_upload_pass_result with_perform_gpu_upload_pass(data_gpu_context &gpu_context, sdl_size_t total_size, l_fill_data_t lambda_fill_data) const;
        template<typename data_t, typename l_fill_data_t>
        proc_gpu& add_count_to_gpu_upload_pass(data_gpu_upload_pass_context &context, sdl_size_t count,
                                               SDL_GPUBuffer &target_buffer, sdl_size_t target_offset, l_fill_data_t lambda_fill_data);

        void destroy_gpu_context(data_gpu_context& inout) const;
        
        std::string create_debug_string(data_gpu_context& context) const;

      protected:
        const char* to_string(SDL_GPUDriver) const;
    };

}