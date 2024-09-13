#include "tod_core/proc_tod.h"
#include "tod_core/poli_tod.h"
#include "tod_core/data_tod.h"

#include <iostream>

#include "tod_core/io.h"

#include "tod_core/data_gpu.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_gpu.h"

#include "tod_core/inc_sdl.h"
#include "tod_core/inc_shadercross.h"
#include "tod_core/assets.h"

using namespace tod;

bool tod::proc_tod::init_tod(poli_tod_init const &p) const
{
	if (!assets::set_main_path(p.asset_main_path) || !assets::set_dep_paths())
	{
		std::cout << "ERROR: Failed to set ToD asset paths!" << std::endl;
		return false;
	}

    SDL_Init(SDL_INIT_VIDEO);
	SDL_ShaderCross_Init();
	return true;
}

bool tod::proc_tod::create_tod_context(data_tod_context &out_tod_context, data_gpu_context &in_gpu_context, poli_tod_context const &) const
{
	tod::proc_gpu		 proc_gpu{};
	tod::poli_gpu_shader poli_gpu_shader{};

	std::string tmp_string;
	
	sugar::slurp_bin(tmp_string, (assets::shader_path / "triangle.vert.spv").string());
	std::cout << "Loading triangle.vert.spv" << std::endl;
	proc_gpu.create_gpu_shader(out_tod_context.vert_shader,
	  in_gpu_context, tod::proc_gpu::create_gpu_shader_instr {
		.stage = SDL_GPU_SHADERSTAGE_VERTEX, .file_data = tmp_string
	  }, poli_gpu_shader
	);
	
	std::cout << "Loading triangle.frag.spv" << std::endl;
	sugar::slurp_bin(tmp_string, (assets::shader_path / "triangle.frag.spv").string());
	proc_gpu.create_gpu_shader(out_tod_context.frag_shader,
	  in_gpu_context, tod::proc_gpu::create_gpu_shader_instr {
	    .stage = SDL_GPU_SHADERSTAGE_FRAGMENT, .file_data = tmp_string
	  }, poli_gpu_shader
	);

    return true;
}

proc_tod::pass_result proc_tod::submit_pass_clear_texture(data_gpu_context& in_context, SDL_GPUTexture& in_texture, const SDL_FColor& colour) const
{
	SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(in_context.device);
	if (cmd_buf == nullptr)
	{
        if (verbose_logging)
          std::cout << "ERROR: Failed to acquire gpu command buffer" << std::endl;
		return pass_result::failure;
	}

	create_pass_clear_cmd(*cmd_buf, in_texture, colour);
	SDL_SubmitGPUCommandBuffer(cmd_buf);

	return pass_result::success;
}

proc_tod::pass_result proc_tod::submit_pass_clear_window(data_gpu_context& in_context, const SDL_FColor& colour) const
{
	SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(in_context.device);
	if (cmd_buf == nullptr)
	{
        if (verbose_logging)
          std::cout << "ERROR: Failed to acquire gpu command buffer" << std::endl;
		return pass_result::failure;
	}

	std::uint32_t w, h;
	SDL_GPUTexture* swapchain_tex = SDL_AcquireGPUSwapchainTexture(cmd_buf, in_context.window, &w, &h);
	if (swapchain_tex)
	  create_pass_clear_cmd(*cmd_buf, *swapchain_tex, colour);

	SDL_SubmitGPUCommandBuffer(cmd_buf);

	return pass_result::success;
}

void proc_tod::create_pass_clear_cmd(SDL_GPUCommandBuffer &in_cmd, SDL_GPUTexture &in_tex, const SDL_FColor &colour) const
{
	SDL_GPUColorTargetInfo color_op = { 0 };
	color_op.texture	 = &in_tex;
	color_op.clear_color = colour;
	color_op.load_op	 = SDL_GPU_LOADOP_CLEAR;
	color_op.store_op	 = SDL_GPU_STOREOP_STORE;

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(&in_cmd, &color_op, 1, NULL);
	SDL_EndGPURenderPass(renderPass);
}