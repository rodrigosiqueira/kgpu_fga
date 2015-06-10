#include "gpuOperations.h"
#include "no_kernel.h"
#include <CL/cl.h>

#include <stdlib.h>

void gpu_init()
{
  static int initialized = 0;
  if (initialized)
  {
    return;
  }
  cl_platform_id platformId = NULL;
  cl_device_id deviceId = NULL;   
  cl_uint retNumDevices;
  cl_uint retNumPlatforms;
  cl_int platformID;
  cl_int deviceID;

  //platformID = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  //deviceID = clGetDeviceIDs( platformID, CL_DEVICE_TYPE_ALL, 1,
  //                             &deviceId, retNumDevices);

 //1 - Alocar memória para troca de informações.
 //2 - Carregar arquivo "openCL":
 // a) Abrir o arquivo .cl
 //   - fopen (arquivo.cl)
 // fopen("kernelOpenCL.cl)
 // b) Alocar memória para salvar o arquivo .cl
 //   - malloc(DEFINIR UM TAMANHO MÁXIMO)
 // c) Ler o arquivo .cl e salvar na memória
 //   - fread, memcpy, qualquer coisa que copia

  //3 - Coletar dados da plataforma:
  //* Para coletar dados da plataforma, o openCL precisa seguir alguns passos:
  //a) cl_platform_id platform_id
  //  b) cl_device_id device_id
  //  c) cl_uint ret_num_devices
  //  d) cl_uint ret_num_platforms
  //  e) clGetPlatformIDs()
  //  f) clGetDeviceIDs()
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
