#include "gpuOperations.h"
#include "no_kernel.h"

#include <stdlib.h>

void gpu_init()
{
  //TODO
}

void gpu_finit()
{
  //TODO
}

void * gpu_alloc_pinned_mem (unsigned long size)
{
  //TODO
}

void gpu_free_pinned_mem (void * p)
{
  //TODO
}


void gpu_pin_mem (void * p, size_t sz)
{
  //TODO
}

void gpu_unpin_mem (void * p)
{
  //TODO
}


int gpu_alloc_device_mem (struct kgpu_service_request * sreq)
{
  //TODO
}

void gpu_free_device_mem (struct kgpu_service_request * sreq)
{
  //TODO
}

int gpu_alloc_stream (struct kgpu_service_request * sreq)
{
  //TODO
}

void gpu_free_stream (struct kgpu_service_request * sreq)
{
  //TODO
}


int gpu_execution_finished (struct kgpu_service_request * sreq)
{
  //TODO
}

int gpu_post_finished (struct kgpu_service_request * sreq)
{
  //TODO
}

unsigned long gpu_get_stream (int sid)
{
  //TODO
  return 0;
}
