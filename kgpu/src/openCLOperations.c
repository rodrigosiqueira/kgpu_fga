#include "gpuOperations.h"
#include "no_kernel.h"
#include <CL/cl.h>

#include <stdlib.h>
#include <stdio.h>

#define MAX_STREAM_NR 8

openCLRuntimeData * openCLData = NULL;

//Translation: cudaStream_t -> cl_command_queue
cl_command_queue streams[MAX_STREAM_NR];
static int streamuses[MAX_STREAM_NR];

//Translation: dim3 -> size_t[3]
static const size_t default_block_size[3]; // 32, 1
static const size_t default_grid_size[3]; // 512, 1

struct kgpu_gpu_mem_info devbuf;
struct kgpu_gpu_mem_info devbuf4vma;

static int initializePlatform()
{
  if (openCLData)
  {
    free(openCLData);
  }

  openCLData = (openCLRuntimeData *) malloc(sizeof(openCLRuntimeData));
  if (!openCLData)
  {
    //TODO: HANDLE IT IN THE RIGHT WAY
    return -1;
  }

  // Retrieve the number of platforms
  cl_uint numPlatforms = 0;
  cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
  // Allocate enough space for each platform
  cl_platform_id * platforms = NULL;
  platforms = (cl_platform_id *) malloc(numPlatforms * sizeof(cl_platform_id));
  //Fill in the platforms
  status = clGetPlatformIDs(numPlatforms, platforms, NULL);

  //DEVICES
  //Retrieve the number of devices
  cl_uint numDevices = 0;
  status = clGetDeviceIDs(platforms[0], 
                          CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
  //Allocate 
  openCLData->devices = (cl_device_id *) 
                        malloc(numDevices * sizeof(cl_device_id));

  //Fill in the devices
  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices,
                          openCLData->devices, NULL);

  // Create a context and associate with the device
  openCLData->context = clCreateContext(NULL, numDevices,
                                        openCLData->devices, NULL,
                                        NULL, &status);
  //TODO: HANDLING THE ERRORS.
  return 0;
}

void gpu_init()
{
  int i = 0;
  static int initialized = 0;
  if (!initialized)
  {
    initializePlatform();
    initialized = 1;
  }

  //Alloc_dev_mem -> calls cudaMalloc. Change it.
  //Translation: cudaMalloc -> clCreateBuffer
  //devbuf.uva = alloc_dev_mem(KGPU_BUF_SIZE);
  cl_int status = 0;
  cl_mem userVirtualAddress = clCreateBuffer (openCLData->context, 
                                             CL_MEM_READ_WRITE,
                                             KGPU_BUF_SIZE, NULL,
                                             &status);
  
  //devbuf4vma.uva = alloc_dev_mem(KGPU_BUF_SIZE);
  cl_mem devBuf_uva = clCreateBuffer (openCLData->context,
                                      CL_MEM_READ_WRITE,
                                      KGPU_BUF_SIZE, NULL,
                                      &status);


  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations: GPU INIT.\n");
  for (i = 0; i < MAX_STREAM_NR; i++) 
  {
    //Translation: cudaStreamCreate -> clCreateCommandQueue
    streams[i] = clCreateCommandQueue(openCLData->context, 
                                      openCLData->devices[0],
                                      CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                                      NULL);
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

cl_mem gpu_alloc_pinned_mem (unsigned long size)
{
  cl_int status;
  //TODO: VERIFY POINTER IN CONTEXT.
  fprintf(stdout, ">>>>> openCLOperation.c: GPU ALLOC.\n");
  //cudaHostAlloc(void ** pHost, size_t    size, unsigned int flagsa
  cl_mem host = clCreateBuffer(openCLData->context, 
                              CL_MEM_ALLOC_HOST_PTR,
                                size, NULL, &status);
  return host;
}

//TODO: It is wrong the parameter. Fix IT!
void gpu_free_pinned_mem (cl_mem memory)
{
  fprintf(stdout, ">>>>> openCLOperation.c: GPU FREE PINNED MEMORY.\n");
  clReleaseMemObject(memory);
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
    sreq->din = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hin);
  }
  else
  {
    sreq->din = (void*)ADDR_REBASE (devbuf4vma.uva, hostvma.uva, sreq->hin);
    pin_addr[npins] = TO_UL(sreq->hin);
    pin_sz[npins] = sreq->insize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hout, hostbuf.uva, hostbuf.size))
  {
    sreq->dout = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hout);
  }
  else
  {
    sreq->dout = (void*)ADDR_REBASE(devbuf4vma.uva, hostvma.uva, sreq->hout);
    pin_addr[npins] = TO_UL(sreq->hout);
    pin_sz[npins] = sreq->outsize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hdata, hostbuf.uva, hostbuf.size))
  {
    sreq->ddata = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hdata);
  }
  else if (ADDR_WITHIN(sreq->hdata, hostvma.uva, hostvma.size))
  {
    sreq->ddata = (void*)ADDR_REBASE(devbuf4vma.uva, hostvma.uva, sreq->hdata);
    pin_addr[npins] = TO_UL(sreq->hdata);
    pin_sz[npins] = sreq->datasize;
    npins++;
  }

  npins = __merge_ranges(pin_addr, pin_sz, npins);
  for (i = 0; i < npins; i++)
  {
    gpu_pin_mem((void*)pin_addr[i], pin_sz[i]);
  }

  return 0;
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
