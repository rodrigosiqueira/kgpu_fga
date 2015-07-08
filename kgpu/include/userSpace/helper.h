/*
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the GPL-COPYING file in the top-level directory.
 *
 * Copyright (c) 2010-2011 University of Utah and the Flux Group.
 * All rights reserved.
 *
 */
 
#ifndef __HELPER_H__
#define __HELPER_H__

#include "kgpu.h"

#define kh_log(level, ...) kgpu_do_log(level, "helper", ##__VA_ARGS__)
#define dbg(...) kh_log(KGPU_LOG_DEBUG, ##__VA_ARGS__)

/**
* ATTENTION: THOSE STRUCTURES WILL BE DEFINED IN HELPEC.C
*/
//extern struct kgpu_gpu_mem_info hostbuf;
//extern struct kgpu_gpu_mem_info hostvma;

/**
* ATTENTION: THOSE STRUCTURES WILL BE DEFINED IN OPENCLOPERATIONS.C AND
*   GPUOPTS.C
*/
//extern struct kgpu_gpu_mem_info devbuf;
//extern struct kgpu_gpu_mem_info devbuf4vma;

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif



//#ifdef __cplusplus
//extern "C" {
//#endif
//
//  void gpu_init(void);
//  void gpu_finit(void);
//
//  void *gpu_alloc_pinned_mem(unsigned long size);
//  void gpu_free_pinned_mem(void * p);
//
//  void gpu_pin_mem(void * p, size_t sz);
//  void gpu_unpin_mem(void * p);
//
//  int gpu_alloc_device_mem(struct kgpu_service_request * sreq);
//  void gpu_free_device_mem(struct kgpu_service_request * sreq);
//  int gpu_alloc_stream(struct kgpu_service_request * sreq);
//  void gpu_free_stream(struct kgpu_service_request * sreq);
//
//  int gpu_execution_finished(struct kgpu_service_request * sreq);
//  int gpu_post_finished(struct kgpu_service_request * sreq);
//
//  unsigned long gpu_get_stream(int sid);
//
//#ifdef __cplusplus
//}
//#endif
   
#endif
