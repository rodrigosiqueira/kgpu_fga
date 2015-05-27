#ifndef _GPU_OPERATION_H_
#define _GPU_OPERATION_H_

#include "no_kernel.h"
#include <stdlib.h>

/*
typedef struct _Gpu_operations
{
  void (*init)(void);
  void (*free)(void);
} Gpu_operations;

extern Gpu_operations gpuOperations;
*/

extern void gpu_init();
extern void gpu_finit();

extern void * gpu_alloc_pinned_mem (unsigned long size);
extern void gpu_free_pinned_mem (void * p);

extern void gpu_pin_mem (void * p, size_t sz);
extern void gpu_unpin_mem (void * p);

extern int gpu_alloc_device_mem (struct kgpu_service_request * sreq);
extern void gpu_free_device_mem (struct kgpu_service_request * sreq);
extern int gpu_alloc_stream (struct kgpu_service_request * sreq);
extern void gpu_free_stream (struct kgpu_service_request * sreq);

extern int gpu_execution_finished (struct kgpu_service_request * sreq);
extern int gpu_post_finished (struct kgpu_service_request * sreq);

extern unsigned long gpu_get_stream (int sid);

#endif
