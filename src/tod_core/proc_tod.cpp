#include "tod_core/proc_tod.h"
#include "tod_core/data_gpu.h"

#include <iostream>

#include "tod_core/inc_sdl.h"

using namespace tod;

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
	SDL_GPUColorAttachmentInfo color_op = { 0 };
	color_op.texture	= &in_tex;
	color_op.clearColor = colour;
	color_op.loadOp	    = SDL_GPU_LOADOP_CLEAR;
	color_op.storeOp	= SDL_GPU_STOREOP_STORE;

	SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(&in_cmd, &color_op, 1, NULL);
	SDL_EndGPURenderPass(renderPass);
}