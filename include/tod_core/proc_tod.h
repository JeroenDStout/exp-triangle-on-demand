#pragma once

/*
 *  ...
 */

 struct SDL_GPUTexture;
 struct SDL_GPUCommandBuffer;
 struct SDL_FColor;

namespace tod {

    struct data_gpu_context;
    struct pol_gpu_context;

    struct proc_tod {
        
        bool verbose_logging = true;
        
        enum class pass_result { success, failure };

        pass_result submit_pass_clear_texture(data_gpu_context& in_context, SDL_GPUTexture& in_texture, const SDL_FColor& colour) const;
        pass_result submit_pass_clear_window(data_gpu_context& in_context, const SDL_FColor& colour) const;

      protected:
        void create_pass_clear_cmd(SDL_GPUCommandBuffer& in_cmd, SDL_GPUTexture& in_tex, const SDL_FColor& colour) const;
    };

}