#include "gputils.h"

static cudaError_t _cuda_safe_call (cudaError_t e, const char * file, int line)
{
  if (e!=cudaSuccess)
  {
    fprintf(stderr, "kgpu Error: %s %d %s\n", file, line,
            cudaGetErrorString(e));
    cudaThreadExit();
    abort();
  }
  fprintf(stdout, "===== gputils.h: CUDA SAFE CALL. [%s]:%d\n", file, line);
  return e;
}

/**
* @param Allocate memory on GPU.
* @return Return a reference of device memory.
*/
static void *alloc_dev_mem(unsigned long size)
{
  void *h;
  csc( cudaMalloc(&h, size) );
  return h;
}

static void free_dev_mem(void * p)
{
  csc( cudaFree(p) );
}
