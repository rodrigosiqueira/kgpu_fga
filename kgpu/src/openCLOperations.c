#include <stdlib.h>
#include <stdio.h>

#include <CL/cl.h>

#include "gpuOperations.h"
#include "no_kernel.h"
#include "dataType.h"

#define MAX_STREAM_NR 8

openCLRuntimeData * openCLData = NULL;

//Translation: cudaStream_t -> cl_command_queue
cl_command_queue streams[MAX_STREAM_NR];
static int streamuses[MAX_STREAM_NR];

//Translation: dim3 -> size_t[3]
static const size_t default_block_size[3]; // 32, 1
static const size_t default_grid_size[3]; // 512, 1

//ATTENTION: FIND A WAY TO DEFINE THIS STRUCTURE HERE, IT IS IMPORTANT
//struct kgpu_gpu_mem_info devbuf;
//struct kgpu_gpu_mem_info devbuf4vma;

kgpuOpenCLGpuMemoryInfo deviceBuffer;
kgpuOpenCLGpuMemoryInfo deviceBufferForVMA;

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

  /* PLATFORM */
  /* 1 -  Retrieve the number of platforms */
  cl_uint numPlatforms = 0;
  cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
  if (status == CL_INVALID_VALUE)
  {
    //TODO: HANDLE IT IN THE RIGHT WAY
    return -1;
  }

  cl_platform_id * platforms = NULL;
  platforms = (cl_platform_id *) malloc(numPlatforms * sizeof(cl_platform_id));
  if (!platforms)
  {
    return -1;
  }

  //Fill in the platforms
  status = clGetPlatformIDs(numPlatforms, platforms, NULL);
  if (status == CL_INVALID_VALUE)
  {
    return -1;
  }

  /* DEVICES */
  /* 2 - Retrieve the number of devices */
  cl_uint numDevices = 0;
  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL,
                          0, NULL, &numDevices);
  // CL_INVALID_PLATFORM, CL_INVALID_DEVICE_TYPE
  if (status == CL_INVALID_VALUE)
  {
    return -1;
  }

  //Allocate 
  openCLData->devices = (cl_device_id *) 
                        malloc(numDevices * sizeof(cl_device_id));
  if (!openCLData->devices)
  {
    return -1;
  }

  //Fill in the devices
  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices,
                          openCLData->devices, NULL);

  /* 3 - Create context*/
  // Create a context and associate with the device
  openCLData->context = clCreateContext(NULL, numDevices,
                                        openCLData->devices, NULL,
                                        NULL, &status);
  //CL_INVALID_PLATFORM, CL_INVALID_VALUE, CL_INVALID_DEVICE,
  // CL_DEVICE_NOT_AVAILABLE, CL_OUT_OF_HOST_MEMORY
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

  //Translation: cudaMalloc -> clCreateBuffer
  //devbuf.uva = alloc_dev_mem(KGPU_BUF_SIZE);
  cl_int status = 0;
  //TODO: CL_INVALID_CONTEXT, CL_INVALID_VALUE, CL_INVALID_BUFFER_SIZE
  //CL_INVALID_HOST_PTR, CL_MEM_OBJECT_ALLOCATION_FAILURE,
  //CL_OUT_OF_HOST_MEMORY
  deviceBuffer.userVirtualAddress = clCreateBuffer (openCLData->context, 
                                                    CL_MEM_READ_WRITE,
                                                    KGPU_BUF_SIZE, NULL,
                                                    &status);
  
  deviceBufferForVMA.userVirtualAddress = clCreateBuffer (openCLData->context,
                                                          CL_MEM_READ_WRITE,
                                                          KGPU_BUF_SIZE, NULL,
                                                          &status);

  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations: GPU INIT.\n");
  for (i = 0; i < MAX_STREAM_NR; i++) 
  {
    //Translation: cudaStreamCreate -> clCreateCommandQueue
    streams[i] = clCreateCommandQueue (openCLData->context, 
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
  //CL_INVALID_MEM_OBJECT
  clReleaseMemObject(deviceBuffer.userVirtualAddress);
  clReleaseMemObject(deviceBufferForVMA.userVirtualAddress);

  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations.c: GPU FINIT.\n");

  for (i = 0; i < MAX_STREAM_NR; i++)
  {
    //TODO: CL_INVALID_COMMAND_QUEUE
    clReleaseCommandQueue(streams[i]);
  }
}

//cl_command_queue gpu_get_stream (int pStreamId)
unsigned long gpu_get_stream (int pStreamId)
{
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> openCLOperation.c: GPU GET STREAM %d\n", pStreamId);
  if (pStreamId < 0 || pStreamId >= MAX_STREAM_NR)
  {
    return 0;
  }
  else
  {
    //TODO: PROBLEM HERE!!! streams is cl_command_stream
    return (unsigned long) streams[pStreamId];
  }
}

cl_mem gpu_alloc_pinned_mem (unsigned long pSize)
{
  cl_int status;
  cl_mem host;

  fprintf(stdout, ">>>>> openCLOperation.c: GPU ALLOC.\n");
  //cudaHostAlloc(void ** pHost, size_t size, unsigned int flagsa
  if (!openCLData->context)
  {
    return host;
  }

  host = clCreateBuffer (openCLData->context, 
                         CL_MEM_ALLOC_HOST_PTR,
                         pSize, NULL, &status);

  //TODO: CL_INVALID_CONTEXT, CL_INVALID_VALUE, CL_INVALID_BUFFER_SIZE,
  //CL_DEVICE_MAX_MEM_ALLOC_SIZE, CL_INVALID_HOST_PTR,
  //CL_MEM_OBJECT_ALLOCATION_FAILURE, CL_OUT_OF_HOST_MEMORY
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

//static int __check_stream_done (cudaStream_t s)
//{
//  cudaError_t e = cudaStreamQuery(s);
//  //TODO: IMPROVE IT
//  fprintf(stdout, ">>>>> gpuops.cu: CHECK STREAM DONE\n");
//  if (e == cudaSuccess) 
//  {
//    return 1;
//  }
//  else if (e != cudaErrorNotReady)
//  {
//    csc(e);
//  }
//
//  return 0;
//}
//
//int gpu_execution_finished (struct kgpu_service_request * sreq)
//{
//  cudaStream_t s = (cudaStream_t) gpu_get_stream (sreq->stream_id);
//  //TODO: IMPROVE IT
//  fprintf(stdout, ">>>>>> gpuops.cu: GPU EXECUTION FINISHED.\n");
//  return __check_stream_done(s);
//}
//
//int gpu_post_finished (struct kgpu_service_request * sreq)
//{
//  fprintf(stdout, ">>>>> gpuops.cu: GPU POST FINISHED\n");
//  cudaStream_t s = (cudaStream_t)gpu_get_stream(sreq->stream_id);
//  return __check_stream_done(s);
//}
//
#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

static int __merge_2ranges(unsigned long r1, unsigned long s1,
                            unsigned long r2, unsigned long s2,
                            unsigned long *e, unsigned long *s)
{
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> gpuops.cu: MERGE 2 RANGES\n");
  // r1   r2
  if (r1 < r2)
  {
    if (r1+s1 >= r2)
    {
      * e = r1;
      * s = max (r1 + s1, r2 + s2) - r1;
      return 1;
    }
    return 0;
  } 
  else if (r1 == r2)
  {
    * e = r1;
    * s = max(s1, s2);
    return 1;
  } 
  else
  {
    // r2  r1
    if (r2 + s2 >= r1)
    {
      * e = r2;
      * s = max(r1 + s1, r2 + s2) - r2;
      return 1;
    }
    return 0;
  }
}

static int __merge_ranges (unsigned long ad[], unsigned long sz[], int n)
{
  int i;

  for (i = 0; i < n; i++)
  {
    ad[i] = round_down(ad[i], PAGE_SIZE);
    sz[i] = round_up(sz[i], PAGE_SIZE);
  }

  //TODO: IMPROVE IT.
  fprintf(stdout, ">>>>> gpuops.cu: Merge ranges.\n");

  switch(n)
  {
    case 0:
      return 0;
    case 1:
      return 1;
    case 2:
      if (__merge_2ranges(ad[0], sz[0], ad[1], sz[1], &ad[0], &sz[0]))
        return 1;
      else
        return 2;
    case 3:
      if (__merge_2ranges(ad[0], sz[0], ad[1], sz[1], &ad[0], &sz[0]))
      {
        if (__merge_2ranges(ad[0], sz[0], ad[2], sz[2], &ad[0], &sz[0])) 
        {
          return 1;
        }
        else
        {
          ad[1] = ad[2];
          sz[1] = sz[2];
          return 2;
        }
      } 
      else if (__merge_2ranges(ad[0], sz[0], ad[2], sz[2], &ad[0], &sz[0]))
      {
        if (__merge_2ranges(ad[0], sz[0], ad[1], sz[1], &ad[0], &sz[0]))
        {
          return 1;
        } 
        else
          return 2;
      }
      else if (__merge_2ranges(ad[2], sz[2], ad[1], sz[1], &ad[1], &sz[1]))
      {
        if (__merge_2ranges(ad[0], sz[0], ad[1], sz[1], &ad[0], &sz[0]))
          return 1;
        else
          return 2;
      }
      else
        return 3;
    default:
      return 0;
  }

    // should never reach here
    //return 0;
}
//
// ....
int gpu_alloc_device_mem (struct kgpu_service_request * sreq)
{
  //TODO
  unsigned long pinAddress[3] = {0, 0, 0}, pinSize[3] = {0, 0, 0};
  int npins = 0, i = 0;

  fprintf(stdout, ">>>>>> openCLOperations.c: GPU ALLOC DEVICE MEMORY.\n");

  if (ADDR_WITHIN(sreq->hin, hostbuf.uva, hostbuf.size))
  {
    //sreq->din = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hin);
    //deviceBuffer.userVirtualAddress
  }
  else
  {
    //sreq->din = (void*)ADDR_REBASE (devbuf4vma.uva, hostvma.uva, sreq->hin);
    //deviceBufferForVMA.userVirtualAddress, deviceBuffer.userVirtualAddress
    pinAddress[npins] = TO_UL(sreq->hin);
    pinSize[npins] = sreq->insize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hout, hostbuf.uva, hostbuf.size))
  {
    //sreq->dout = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hout);
    //deviceBuffer.userVirtualAddress
  }
  else
  {
    //sreq->dout = (void*)ADDR_REBASE(devbuf4vma.uva, hostvma.uva, sreq->hout);
    //deviceBufferForVMA
    pinAddress[npins] = TO_UL(sreq->hout);
    pinSize[npins] = sreq->outsize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hdata, hostbuf.uva, hostbuf.size))
  {
    //sreq->ddata = (void*)ADDR_REBASE(devbuf.uva, hostbuf.uva, sreq->hdata);
    //deviceBuffer.userVirtualAddress
  }
  else if (ADDR_WITHIN(sreq->hdata, hostvma.uva, hostvma.size))
  {
    //sreq->ddata = 
    //(void*)ADDR_REBASE(devbuf4vma.uva, hostvma.uva, sreq->hdata);
    //deviceBufferForVMA.userVirtualAddress
    pinAddress[npins] = TO_UL(sreq->hdata);
    pinSize[npins] = sreq->datasize;
    npins++;
  }

  npins = __merge_ranges(pinAddress, pinSize, npins);
  for (i = 0; i < npins; i++)
  {
    gpu_pin_mem((void*)pinAddress[i], pinSize[i]);
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

