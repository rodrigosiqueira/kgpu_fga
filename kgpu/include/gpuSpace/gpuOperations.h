#ifndef _GPU_OPERATION_H_
#define _GPU_OPERATION_H_

#include <stdlib.h>

#include <CL/cl.h>
#include "no_kernel.h"

/*
typedef struct _Gpu_operations
{
  void (*init)(void);
  void (*free)(void);
} Gpu_operations;

extern Gpu_operations gpuOperations;
*/

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

typedef struct _openCLRuntimeData
{
  cl_context  context;
  cl_device_id * devices;
}openCLRuntimeData;

typedef struct 
{
  cl_mem userVirtualAddress;
}kgpuOpenCLGpuMemoryInfo;

extern openCLRuntimeData * openCLData;

/**
* @brief Initialize OpenCL and KGPU resources.
*/
extern void gpu_init();

/**
* @brief Release openCL resources.
*/
extern void gpu_finit();

/**
* @param pSid
* @return 
* @brief
*/
extern cl_command_queue gpu_get_stream (int pSid);

/**
* @param pSize Memory size.
* @return Return an cl_mem reference.
* @brief Allocates page-locked memory on the host. 
*/
extern void * gpu_alloc_pinned_mem (unsigned long pSize);
extern void gpu_free_pinned_mem (cl_mem memory);

extern void gpu_pin_mem (void * p, size_t sz);
extern void gpu_unpin_mem (void * p);

extern int gpu_alloc_device_mem (struct kgpu_service_request * sreq);
extern void gpu_free_device_mem (struct kgpu_service_request * sreq);
extern int gpu_alloc_stream (struct kgpu_service_request * sreq);
extern void gpu_free_stream (struct kgpu_service_request * sreq);

extern int gpu_execution_finished (struct kgpu_service_request * sreq);
extern int gpu_post_finished (struct kgpu_service_request * sreq);


#endif
