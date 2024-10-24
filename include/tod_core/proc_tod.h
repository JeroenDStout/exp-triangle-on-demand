#pragma once

/*
 *  ...
 */

 struct SDL_GPUTexture;
 struct SDL_GPUCommandBuffer;
 struct SDL_FColor;

namespace tod {

    struct data_tod_context;
    struct data_gpu_context;
    struct poli_tod_init;
    struct poli_tod_context;

    struct proc_tod {
        
        bool verbose_logging = true;
        
        bool init_tod(poli_tod_init const &) const;

        bool create_tod_context(data_tod_context &out_tod_context, data_gpu_context &in_gpu_context, poli_tod_context const &) const;

        enum class pass_result { success, failure };
        pass_result submit_pass_clear_texture(data_gpu_context &, SDL_GPUTexture &in_texture, SDL_FColor const &clear_colour) const;
        pass_result submit_pass_clear_window(data_gpu_context &, SDL_FColor const &clear_colour) const;
        pass_result submit_pass_render_triangle_to_texture(data_gpu_context &, data_tod_context &, SDL_GPUTexture &in_texture, SDL_FColor const &clear_colour) const;
        pass_result submit_pass_render_triangle_to_window(data_gpu_context &, data_tod_context &, SDL_FColor const &clear_colour) const;

      protected:
        void create_pass_clear_cmd(SDL_GPUCommandBuffer &in_cmd, SDL_GPUTexture &in_tex, SDL_FColor const &colour) const;
        void create_pass_draw_triangle(SDL_GPUCommandBuffer &in_cmd, data_tod_context &, SDL_GPUTexture &in_tex, SDL_FColor const &clear_colour) const;
    };

}