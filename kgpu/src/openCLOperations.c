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
  int i = 0;

  //Alloc_dev_mem -> calls cudaMalloc. Change it.
  //Translation: cudaMalloc -> clCreateBuffer
  //devbuf.uva = alloc_dev_mem(KGPU_BUF_SIZE);
  cl_mem userVirtualAddress = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                            datasize, NULL, &status);
  //devbuf4vma.uva = alloc_dev_mem(KGPU_BUF_SIZE);
  cl_mem devBuf_uva = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                            datasize, NULL, &status);


  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations: GPU INIT.\n");
  for (i = 0; i < MAX_STREAM_NR; i++) 
  {
    //Translation: cudaStreamCreate -> clCreateCommandQueue
    streams[i] = clCreateCommandQueue(context, device,
                    CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE , NULL);
    streamuses[i] = 0;
  }
}

void gpu_finit()
{
  int i = 0;

  //free_dev_mem (devbuf.uva);
  //free_dev_mem (devbuf.uva);
  clReleaseMemObject(devbuf.uva);
  clReleaseMemObject(devbuf.uva);

  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations.c: GPU FINIT.\n");

  for (i = 0; i < MAX_STREAM_NR; i++)
  {
    clReleaseCommandQueue(streams[i]);
  }
}

unsigned long gpu_get_stream (int stid)
{
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> openCLOperation.c: GPU GET STREAM %d\n", stid);
  if (stid < 0 || stid >= MAX_STREAM_NR)
  {
    return 0;
  }
  else
  {
    return (unsigned long) streams[stid];
  }
}


void * gpu_alloc_pinned_mem (unsigned long size)
{
  void * host = 0;

  fprintf(stdout, ">>>>> openCLOperation.c: GPU ALLOC.\n");
  //cudaHostAlloc(void ** pHost, size_t    size, unsigned int flagsa
  cl_mem host = clCreateBuffer(context, CL_MEM_ALLOC_HOST_PTR,
                                datasize, NULL, &status);
  //TODO: NOT GOOD! IT IS WRONG THE DATATYPE.
  return host;
}

//TODO: It is wrong the parameter. Fix IT!
void gpu_free_pinned_mem (void * p)
{
  cl_mem host;
  fprintf(stdout, ">>>>> openCLOperation.c: GPU FREE PINNED MEMORY.\n");
  clReleaseMemObject(host);
}

//TODO: HUGE PROBLEM! I DON'T KNOW HOW TO CONVERT cudaHostRegister
void gpu_pin_mem (void * p, size_t sz)
{
  fprintf(stdout, ">>>>> openCLOperations.c: GPU PIN MEMORY\n");
  size_t rsz = round_up(sz, PAGE_SIZE);
  //csc( cudaHostRegister(p, rsz, cudaHostRegisterPortable) );
}

void gpu_unpin_mem (void * p)
{
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> openCLOperations.c: GPU UNPIN MEMORY\n");
  //csc( cudaHostUnregister(p) );
}

//TODO: YOU MISSED SOME FUNCTIONS. 

int gpu_alloc_device_mem (struct kgpu_service_request * sreq)
{
  //TODO
  unsigned long pinAddress[3] = {0, 0, 0}, pinSize[3] = {0, 0, 0};
  int npins = 0, i = 0;

  fprintf(stdout, ">>>>>> openCLOperations.c: GPU ALLOC DEVICE MEMORY.\n");

  if (ADDR_WITHIN(sreq->hin, hostbuf.uva, hostbuf.size))
  {
    
  }
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
