#pragma once

#include "proc_gpu.h"
#include "inc_sdl.h"
#include "refvalue.h"

struct tod::proc_gpu::data_gpu_upload_pass_context {
    data_gpu_context      *context;
    SDL_GPUCopyPass       *copy_pass;
    SDL_GPUTransferBuffer *transfer_buffer;
    std::byte             *data  = nullptr;
    sdl_size_t            byte_offset = 0;
};

template<typename l_fill_data_t>
auto tod::proc_gpu::with_perform_gpu_upload_pass(data_gpu_context &gpu_context, sdl_size_t total_size, l_fill_data_t lambda_fill_data) const -> perform_gpu_upload_pass_result
{
	namespace ts = tod::sugar;

	// To set up, first we create a transfer buffer...
    SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(gpu_context.device,
	  &ts::keep(SDL_GPUTransferBufferCreateInfo{
	    .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
	    .size = total_size
	  })
	);
    if (transfer_buffer == nullptr)
      return perform_gpu_upload_pass_result::failure;
	  
	// ...which we map...
	void *transfer_data = SDL_MapGPUTransferBuffer(gpu_context.device, transfer_buffer, false);
    if (transfer_data == nullptr)
      return perform_gpu_upload_pass_result::failure;
	  
	// ...and create a copy pass command buffer for
	SDL_GPUCommandBuffer *upload_command_buffer = SDL_AcquireGPUCommandBuffer(gpu_context.device);
	SDL_GPUCopyPass      *copy_pass             = SDL_BeginGPUCopyPass(upload_command_buffer);

	// We let the caller fill in the data using a context
	lambda_fill_data(ts::keep(data_gpu_upload_pass_context{
	  .context		   = &gpu_context,
	  .copy_pass	   = copy_pass,
	  .transfer_buffer = transfer_buffer,
	  .data            = (std::byte*)transfer_data,
	  .byte_offset	   = 0
	}));
	
	// To finish up, first we unmap the buffer...
	SDL_UnmapGPUTransferBuffer(gpu_context.device, transfer_buffer);

	// ...finish & submit the copy pass...
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_command_buffer);

	// ...and release the transfer buffer
	SDL_ReleaseGPUTransferBuffer(gpu_context.device, transfer_buffer);

	return perform_gpu_upload_pass_result::success;
}

template<typename data_t, typename l_fill_data_t>
auto tod::proc_gpu::add_count_to_gpu_upload_pass(data_gpu_upload_pass_context &context, sdl_size_t count,
											     SDL_GPUBuffer& target_buffer, sdl_size_t target_offset, l_fill_data_t lambda_fill_data) -> proc_gpu&
{
	namespace ts = tod::sugar;

	sdl_size_t addition_size = sizeof(data_t) * count;

	lambda_fill_data((data_t*)(context.data + context.byte_offset));

	SDL_UploadToGPUBuffer(
	  context.copy_pass,
	  &ts::keep(SDL_GPUTransferBufferLocation{
	  	.transfer_buffer = context.transfer_buffer,
	  	.offset			 = target_offset
	  }),
	  &ts::keep(SDL_GPUBufferRegion{
	  	.buffer          = &target_buffer,
	  	.offset			 = target_offset,
	  	.size			 = addition_size
	  }),
	  false
	);

	context.byte_offset += addition_size;

	return *this;
}