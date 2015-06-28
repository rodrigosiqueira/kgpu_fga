/*
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the GPL-COPYING file in the top-level directory.
 *
 * Copyright (c) 2010-2011 University of Utah and the Flux Group.
 * All rights reserved.
 *
 * Common header for userspace helper, kernel mode KGPU and KGPU clients
 *
 */

#ifndef __KGPU_H__
#define __KGPU_H__

#define TO_UL(v) ((unsigned long)(v))

/**
* @def ADDR_WITHIN
* @brief This macro, just check if pointer is in the correct range.
*/
#define ADDR_WITHIN(pointer, base, size)      \
  (TO_UL(pointer) >= TO_UL(base) &&			      \
  (TO_UL(pointer) < TO_UL(base)+TO_UL(size)))

/**
* @def ADDR_REBASE
* @brief Rebase pointer.
*/
#define ADDR_REBASE(dst_base, src_base, pointer)  \
  (TO_UL(dst_base) + (TO_UL(pointer) - TO_UL(src_base)))

struct kgpu_gpu_mem_info 
{
  void * uva;
  unsigned long size;
};

#define KGPU_SERVICE_NAME_SIZE 32
// TODO: Comment each field.
struct kgpu_ku_request 
{
  int id;
  char service_name[KGPU_SERVICE_NAME_SIZE];
  void * in; 
  void * out;
  void * data;
  unsigned long insize; 
  unsigned long outsize; 
  unsigned long datasize;
};

/* kgpu's errno */
// TODO: Replace it for enum.
#define KGPU_OK 0
#define KGPU_NO_RESPONSE 1
#define KGPU_NO_SERVICE 2
#define KGPU_TERMINATED 3

struct kgpu_ku_response 
{
  int id;
  int errcode;
};

/* TODO: If it is necessary only for helper, put it in another header!
 * For helper and service providers
 */
#if defined __KERNEL__ || defined __KGPU__
    #include "kernel_code.h"
#endif

#if defined __KERNEL__
    #include "kgpu_kernel.h"
#else
    #include "no_kernel.h"
#endif

#endif
