#ifndef _CUDA_OPS_H_
#define _CUDA_OPS_H_

extern "C" void gpu_init();
extern "C" void gpu_finit();

extern "C" void * gpu_alloc_pinned_mem (unsigned long size);
extern "C" void gpu_free_pinned_mem (void * p);

extern "C" void gpu_pin_mem (void * p, size_t sz);
extern "C" void gpu_unpin_mem (void * p);

extern "C" int gpu_alloc_device_mem (struct kgpu_service_request * sreq);
extern "C" void gpu_free_device_mem (struct kgpu_service_request * sreq);
extern "C" int gpu_alloc_stream (struct kgpu_service_request * sreq);
extern "C" void gpu_free_stream (struct kgpu_service_request * sreq);

extern "C" int gpu_execution_finished (struct kgpu_service_request * sreq);
extern "C" int gpu_post_finished (struct kgpu_service_request * sreq);

extern "C" unsigned long gpu_get_stream (int sid);

#endif
