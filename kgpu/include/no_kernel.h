#ifndef _NO_KERNEL_H_
#define _NO_KERNEL_H_

struct kgpu_service;

/**
* @struct kgpu_service_request
* @brief This structure is one of the core in KGPU. Here, we have the reference
*        between host and device.
*/
struct kgpu_service_request 
{
  int id;
  void * hin;               /**< Host memory input.*/
  void * hout;              /**< Host memory output.*/
  void * hdata;             /**< Host data.*/
  void * din;               /**< Device memory input.*/
  void * dout;              /**< Device memory output.*/
  void * ddata;             /**< Device data*/
  unsigned long insize;
  unsigned long outsize;
  unsigned long datasize;
  int errcode;
  struct kgpu_service * s;
  int block_x;
  int block_y;
  int grid_x;
  int grid_y;
  int state;
  int stream_id;
  unsigned long stream;
};

// TODO: Create enum to replace it.
/* Service request states: */
#define KGPU_REQ_INIT 1
#define KGPU_REQ_MEM_DONE 2
#define KGPU_REQ_PREPARED 3
#define KGPU_REQ_RUNNING 4
#define KGPU_REQ_POST_EXEC 5
#define KGPU_REQ_DONE 6

#include "service.h"

#endif
