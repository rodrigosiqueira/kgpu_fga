#include "gpuOperations.h"
#include "no_kernel.h"
#include <CL/cl.h>

#include <stdlib.h>

#define MAX_STREAM_NR 8
//Translation: cudaStream_t -> cl_command_queue
cl_command_queue streams[MAX_STREAM_NR];
static int streamuses[MAX_STREAM_NR];
//Translation: dim3 -> size_t[3]
static const size_t default_block_size[3]; // 32, 1
static const size_t default_grid_size[3]; // 512, 1

struct kgpu_gpu_mem_info devbuf;
struct kgpu_gpu_mem_info devbuf4vma;

void gpu_init()
{
  int i;

  devbuf.uva = alloc_dev_mem(KGPU_BUF_SIZE);
//  devbuf4vma.uva = alloc_dev_mem(KGPU_BUF_SIZE);

  //TODO: Improve it.
//  fprintf(stdout, ">>>>> gpuops.cu: GPU INIT.\n");

//  for (i = 0; i < MAX_STREAM_NR; i++) 
//  {
//    csc( cudaStreamCreate(&streams[i]) );
//    streamuses[i] = 0;
//  }


//OPENCL AREA
  static int initialized = 0;
  if (initialized)
  {
    return;
  }

  // Alloc memory.
  devbuf.uva = malloc(KGPU_BUF_SIZE);
  devbuf4vma.uva = malloc(KGPU_BUF_SIZE);
  // Creates a new asynchronous stream. cudaStreaCreate

  initialized = 1;
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
