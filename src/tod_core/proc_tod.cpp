#include "tod_core/proc_tod.h"
#include "tod_core/poli_tod.h"
#include "tod_core/data_tod.h"

#include <iostream>

#include "tod_core/io.h"
#include "tod_core/refvalue.h"

#include "tod_core/data_gpu.h"
#include "tod_core/data_vert.h"
#include "tod_core/poli_gpu.h"
#include "tod_core/proc_gpu.h"
#include "tod_core/proc_gpu_impl.h"

#include "tod_core/inc_sdl.h"
#include "tod_core/inc_shadercross.h"
#include "tod_core/assets.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4566) // character represented by universal-character-name (...) cannot be represented in the current code page
#endif

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
	namespace ts = tod::sugar;

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

	out_tod_context.vert_buffer = SDL_CreateGPUBuffer(
      in_gpu_context.device,
	  &ts::keep(SDL_GPUBufferCreateInfo{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = tod::vert::pos_col_size * 3
	  })
    );
	proc_gpu.with_perform_gpu_upload_pass(in_gpu_context,
	  tod::vert::pos_col_size * 3,
	  [&proc_gpu, &out_tod_context](tod::proc_gpu::data_gpu_upload_pass_context& context) {
		proc_gpu.add_count_to_gpu_upload_pass<tod::vert::pos_col>(context, 3, *out_tod_context.vert_buffer, 0, [](tod::vert::pos_col* data) {
	      data[0] = {                   0.f,  0.75f, 0xFF, 0xFF, 0x00, 0x00 };
	      data[1] = {  std::sqrt(3.f) / 2.f, -0.75f, 0xFF, 0x00, 0xFF, 0x00 };
	      data[2] = { -std::sqrt(3.f) / 2.f, -0.75f, 0xFF, 0x00, 0x00, 0xFF };
		});
	});

	out_tod_context.pipeline = SDL_CreateGPUGraphicsPipeline(in_gpu_context.device, &sugar::keep(SDL_GPUGraphicsPipelineCreateInfo{
	  .vertex_shader   = out_tod_context.vert_shader,
	  .fragment_shader = out_tod_context.frag_shader,
	  .vertex_input_state = {
		.vertex_buffer_descriptions = sugar::make_array<SDL_GPUVertexBufferDescription>(
		  SDL_GPUVertexBufferDescription{
			.slot			    = 0,
			.pitch				= tod::vert::pos_col_size,
			.input_rate		    = SDL_GPU_VERTEXINPUTRATE_VERTEX,
			.instance_step_rate = 0
		  }
		).data(),
		.num_vertex_buffers = 1,
		.vertex_attributes = sugar::make_array<SDL_GPUVertexAttribute>(
		  SDL_GPUVertexAttribute{
			.location			= 0,
			.buffer_slot		= 0,
			.format				= SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset				= 0
		  },
		  SDL_GPUVertexAttribute{
			.location			= 1,
			.buffer_slot		= 0,
			.format				= SDL_GPU_VERTEXELEMENTFORMAT_UBYTE4_NORM,
			.offset				= sizeof(float) * 3
		  }
		).data(),
		.num_vertex_attributes = 2
	  },
	  .primitive_type      = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
	  .rasterizer_state    = SDL_GPURasterizerState{},
	  .multisample_state   = SDL_GPUMultisampleState{},
	  .depth_stencil_state = SDL_GPUDepthStencilState{},
	  .target_info = {
		.color_target_descriptions = sugar::make_array<SDL_GPUColorTargetDescription>(
		  SDL_GPUColorTargetDescription{
			.format      = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
			.blend_state = {
			  .enable_blend = false
			}
		  }
		).data(),
		.num_color_targets = 1
	  }
	}));

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
	SDL_GPUTexture *swapchain_tex;
	if (SDL_AcquireGPUSwapchainTexture(cmd_buf, in_context.window, &swapchain_tex, &w, &h))
	  create_pass_clear_cmd(*cmd_buf, *swapchain_tex, colour);

	SDL_SubmitGPUCommandBuffer(cmd_buf);

	return pass_result::success;
}

proc_tod::pass_result tod::proc_tod::submit_pass_render_triangle_to_texture(data_gpu_context &gpu_context, data_tod_context &tod_context, SDL_GPUTexture &in_tex, SDL_FColor const &clear_colour) const
{
	SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(gpu_context.device);
	if (cmd_buf == nullptr)
	{
        if (verbose_logging)
          std::cout << "ERROR: Failed to acquire gpu command buffer" << std::endl;
		return pass_result::failure;
	}

	create_pass_draw_triangle(*cmd_buf, tod_context, in_tex, clear_colour);

	SDL_SubmitGPUCommandBuffer(cmd_buf);

	return pass_result::success;
}

proc_tod::pass_result tod::proc_tod::submit_pass_render_triangle_to_window(data_gpu_context &gpu_context, data_tod_context &tod_context, SDL_FColor const &clear_colour) const
{
	SDL_GPUCommandBuffer* cmd_buf = SDL_AcquireGPUCommandBuffer(gpu_context.device);
	if (cmd_buf == nullptr)
	{
        if (verbose_logging)
          std::cout << "ERROR: Failed to acquire gpu command buffer" << std::endl;
		return pass_result::failure;
	}

	std::uint32_t w, h;
	SDL_GPUTexture *swapchain_tex;
	SDL_AcquireGPUSwapchainTexture(cmd_buf, gpu_context.window, &swapchain_tex, &w, &h);

	create_pass_draw_triangle(*cmd_buf, tod_context, *swapchain_tex, clear_colour);

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

void proc_tod::create_pass_draw_triangle(SDL_GPUCommandBuffer &cmd, data_tod_context &tod_context, SDL_GPUTexture &in_tex, SDL_FColor const &clear_colour) const
{
	auto render_pass = SDL_BeginGPURenderPass(&cmd,
	  sugar::make_array<SDL_GPUColorTargetInfo>(
		SDL_GPUColorTargetInfo{ .texture = &in_tex, .clear_color = clear_colour, .load_op = SDL_GPU_LOADOP_CLEAR }
	  ).data(), 1, nullptr
	);
	SDL_BindGPUGraphicsPipeline(render_pass, tod_context.pipeline);
	SDL_BindGPUVertexBuffers(render_pass, 0, sugar::make_array<SDL_GPUBufferBinding>(
	  SDL_GPUBufferBinding{ .buffer = tod_context.vert_buffer, .offset = 0 }
	  ).data(), 1
	);
	SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
	SDL_EndGPURenderPass(render_pass);
}

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif
