#include <stdlib.h>
#include <stdio.h>

#include <CL/cl.h>

#include "gpuOperations.h"
#include "no_kernel.h"
#include "dataType.h"
#include "utils/error.h"

#define MAX_STREAM_NR 8

openCLRuntimeData * openCLData = NULL;

cl_command_queue streams[MAX_STREAM_NR];
static int streamuses[MAX_STREAM_NR];

static const size_t default_block_size[3]; // 32, 1
static const size_t default_grid_size[3]; // 512, 1

//ATTENTION: FIND A WAY TO DEFINE THIS STRUCTURE HERE, IT IS IMPORTANT
//struct kgpu_gpu_mem_info devbuf;
//struct kgpu_gpu_mem_info devbuf4vma;

kgpuOpenCLGpuMemoryInfo deviceBuffer;
kgpuOpenCLGpuMemoryInfo deviceBufferForVMA;

static int initializePlatform(cl_platform_id * platforms)
{
  cl_uint numPlatforms = 0;
  cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
  if (status != CL_SUCCESS)
  {
    return status;
  }

  platforms = (cl_platform_id *) malloc(numPlatforms * sizeof(cl_platform_id));
  if (!platforms)
  {
    return NO_SPACE_ON_HOST;
  }

  //Fill in the platforms
  status = clGetPlatformIDs(numPlatforms, platforms, NULL);
  if (status != CL_SUCCESS)
  {
    return status;
  }

  return status;
}

static int initializeDevice(cl_platform_id * platforms,
                            openCLRuntimeData * openCLData,
                            cl_uint * pNumDevices)
{
  cl_uint numDevices = 0;
  cl_int status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL,
                          0, NULL, &numDevices);
  // CL_INVALID_PLATFORM, CL_INVALID_DEVICE_TYPE
  if (status != CL_SUCCESS)
  {
    return status;
  }

  //Allocate 
  openCLData->devices = (cl_device_id *) 
                        malloc(numDevices * sizeof(cl_device_id));
  if (!openCLData->devices)
  {
    status = NO_SPACE_ON_HOST;
    return status;
  }

  //Fill in the devices
  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, numDevices,
                          openCLData->devices, NULL);
  if (status != CL_SUCCESS)
  {
    return status;
  }

  *pNumDevices = numDevices;
  return status;
}

static int initializeContext(openCLRuntimeData * openCLData, cl_uint pDevices)
{
  // Create a context and associate with the device
  cl_int status = 0;
  openCLData->context = clCreateContext(NULL, pDevices,
                                        openCLData->devices, NULL,
                                        NULL, &status);
  return status;
}

static int initializeOpenCL()
{
  cl_int status = 0;

  if (openCLData)
  {
    free(openCLData);
  }

  openCLData = (openCLRuntimeData *) malloc(sizeof(openCLRuntimeData));
  if (!openCLData)
  {
    printErrorMessage(NO_SPACE_ON_HOST);
    return NO_SPACE_ON_HOST;
  }

  cl_platform_id * platforms = NULL;

  status = initializePlatform(platforms);
  if(status != CL_SUCCESS)
  {
    goto firstLevelOfClean;
  }

  cl_uint numDevices = 0;
  status = initializeDevice(platforms, openCLData, &numDevices);
  if (status != CL_SUCCESS)
  {
    goto secondLevelOfClean;
  }

  status = initializeContext(openCLData, numDevices);
  if (status != CL_SUCCESS)
  {
    goto thirdLevelOfClean;
  }

  return status;

  thirdLevelOfClean:
    free(openCLData->devices);

  secondLevelOfClean:
    free(platforms);

  firstLevelOfClean:
    free(openCLData);
  
  printErrorMessage(status);
  return status;
}

void gpu_init()
{
  int i = 0;
  static int initialized = 0;
  cl_int status = 0;

  if (!initialized)
  {
    status = initializeOpenCL ();
    if (status != CL_SUCCESS)
    {
      return;
    }
    initialized = 1;
  }

  deviceBuffer.userVirtualAddress = clCreateBuffer (openCLData->context,
                                                    CL_MEM_READ_WRITE,
                                                    KGPU_BUF_SIZE, NULL,
                                                    &status);

  if (status != CL_SUCCESS)
  {
    printErrorMessage(status);
    return;
  }

  deviceBufferForVMA.userVirtualAddress = clCreateBuffer (openCLData->context,
                                                          CL_MEM_READ_WRITE,
                                                          KGPU_BUF_SIZE, NULL,
                                                          &status);

  if (status != CL_SUCCESS)
  {
    printErrorMessage(status);
    return;
  }

  for (i = 0; i < MAX_STREAM_NR; i++) 
  {
    streams[i] = clCreateCommandQueue (openCLData->context,
                                       openCLData->devices[0],
                                       CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE,
                                       NULL);
    streamuses[i] = 0;
  }

  return;
}

void gpu_finit()
{
  int i = 0;
  cl_int status = 0;

  //CL_INVALID_MEM_OBJECT
  status = clReleaseMemObject(deviceBuffer.userVirtualAddress);
  if (status != CL_SUCCESS)
  {
    printErrorMessage(status);
    return;
  }

  status = clReleaseMemObject(deviceBufferForVMA.userVirtualAddress);
  if (status != CL_SUCCESS)
  {
    printErrorMessage(status);
    return;
  }

  //TODO: Improve it.
  fprintf(stdout, ">>>>> openCLOperations.c: GPU FINIT.\n");

  for (i = 0; i < MAX_STREAM_NR; i++)
  {
    //TODO: CL_INVALID_COMMAND_QUEUE
    status = clReleaseCommandQueue(streams[i]);
    if (status != CL_SUCCESS)
    {
      printErrorMessage(status);
    }
  }

  free(openCLData);
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
  if (status != CL_SUCCESS)
  {
    printErrorMessage(status);
    return host;
  }

  return host;
}

//TODO: It is wrong the parameter. Fix IT!
void gpu_free_pinned_mem (cl_mem memory)
{
  fprintf(stdout, ">>>>> openCLOperation.c: GPU FREE PINNED MEMORY.\n");
  cl_int status = clReleaseMemObject(memory);
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

static int __check_stream_done (cl_command_queue stream)
{
  //TODO: TRANSLATE IT
  //cudaError_t e = cudaStreamQuery(s);
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> gpuops.cu: CHECK STREAM DONE\n");
  //if (e == cudaSuccess) 
  //{
  //  return 1;
  //}
  //else if (e != cudaErrorNotReady)
  //{
  //  csc(e);
  //}

  return 0;
}

int gpu_execution_finished (struct kgpu_service_request * sreq)
{
  //TODO: TRANSLATE IT
  //cudaStream_t s = (cudaStream_t) gpu_get_stream (sreq->stream_id);
  cl_command_queue s;
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>>> gpuops.cu: GPU EXECUTION FINISHED.\n");
  return __check_stream_done(s);
}

int gpu_post_finished (struct kgpu_service_request * sreq)
{
  fprintf(stdout, ">>>>> gpuops.cu: GPU POST FINISHED\n");
  //TODO: TRANSLATE IT
  //cudaStream_t s = (cudaStream_t)gpu_get_stream(sreq->stream_id);
  cl_command_queue s;
  return __check_stream_done(s);
}

#define min(a,b) (((a)<(b))?(a):(b))
#define max(a,b) (((a)>(b))?(a):(b))

static int __merge_2ranges (unsigned long r1, unsigned long s1,
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

int gpu_alloc_device_mem (struct kgpu_service_request * sreq)
{
  //TODO
  unsigned long pinAddress[3] = {0, 0, 0}, pinSize[3] = {0, 0, 0};
  int npins = 0, i = 0;
  void * deviceBufferTmp = 0, * deviceBufferForVMATmp = 0;
  cl_int status;

  status = clGetMemObjectInfo (deviceBuffer.userVirtualAddress,
                               CL_MEM_HOST_PTR,
                               sizeof(void *),
                               deviceBufferTmp, NULL);

  status = clGetMemObjectInfo (deviceBufferForVMA.userVirtualAddress,
                               CL_MEM_HOST_PTR,
                               sizeof(void *),
                               deviceBufferForVMATmp, NULL);

  fprintf(stdout, ">>>>>> openCLOperations.c: GPU ALLOC DEVICE MEMORY.\n");
  //INPUT
  if (ADDR_WITHIN(sreq->hin, hostbuf.uva, hostbuf.size))
  {
    sreq->din = (void*)ADDR_REBASE(deviceBufferTmp, hostbuf.uva, sreq->hin);
  }
  else
  {
    sreq->din = (void*)ADDR_REBASE (deviceBufferForVMATmp,
                                    hostvma.uva, sreq->hin);
    pinAddress[npins] = TO_UL(sreq->hin);
    pinSize[npins] = sreq->insize;
    npins++;
  }

  //OUTPUT
  if (ADDR_WITHIN(sreq->hout, hostbuf.uva, hostbuf.size))
  {
    sreq->dout = (void*)ADDR_REBASE(deviceBufferTmp, hostbuf.uva, sreq->hout);
  }
  else
  {
    sreq->dout = (void*)ADDR_REBASE(deviceBufferForVMATmp,
                                    hostvma.uva, sreq->hout);
    pinAddress[npins] = TO_UL(sreq->hout);
    pinSize[npins] = sreq->outsize;
    npins++;
  }

  //DATA
  if (ADDR_WITHIN(sreq->hdata, hostbuf.uva, hostbuf.size))
  {
    sreq->ddata = (void*)ADDR_REBASE(deviceBufferTmp,
                                     hostbuf.uva, sreq->hdata);
  }
  else if (ADDR_WITHIN(sreq->hdata, hostvma.uva, hostvma.size))
  {
    sreq->ddata = (void*)ADDR_REBASE(deviceBufferForVMATmp,
                                     hostvma.uva, sreq->hdata);
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
  unsigned long pin_addr[3] = {0,0,0}, pin_sz[3] = {0,0,0};
  int npins = 0, i;

  sreq->din = NULL;
  sreq->dout = NULL;
  sreq->ddata = NULL;

  // TODO: Improve it.
  fprintf(stdout, ">>>>> gpuops.cu: Free device memory.\n");

  if (ADDR_WITHIN(sreq->hin, hostvma.uva, hostvma.size))
  {
    pin_addr[npins] = TO_UL(sreq->hin);
    pin_sz[npins] = sreq->insize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hout, hostvma.uva, hostvma.size))
  {
    pin_addr[npins] = TO_UL(sreq->hout);
    pin_sz[npins] = sreq->outsize;
    npins++;
  }

  if (ADDR_WITHIN(sreq->hdata, hostvma.uva, hostvma.size))
  {
    pin_addr[npins] = TO_UL(sreq->hdata);
    pin_sz[npins] = sreq->datasize;
    npins++;
  }

  npins = __merge_ranges(pin_addr, pin_sz, npins);
  for (i = 0; i < npins; i++)
  {
    gpu_unpin_mem((void*)pin_addr[i]);
  }
}

int gpu_alloc_stream (struct kgpu_service_request * sreq)
{
  int i;

  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> gpuops.cu: GPU ALLOC STREAM\n");

  for (i = 0; i < MAX_STREAM_NR; i++)
  {
    if (!streamuses[i])
    {
      streamuses[i] = 1;
      sreq->stream_id = i;
      sreq->stream = (unsigned long)(streams[i]);
      return 0;
    }
  }
  return 1;
}

void gpu_free_stream (struct kgpu_service_request * sreq)
{
  //TODO: IMPROVE IT
  fprintf(stdout, ">>>>> gpuops.cu: GPU FREE STREAM\n");

  if (sreq->stream_id >= 0 && sreq->stream_id < MAX_STREAM_NR)
  {
    streamuses[sreq->stream_id] = 0;
  }
}
