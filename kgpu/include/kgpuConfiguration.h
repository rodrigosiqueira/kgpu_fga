#ifndef _KGPU_CONFIGURATION_H_
#define _KGPU_CONFIGURATION_H_

/* the NR will not be used */
#define KGPU_BUF_NR 1

/* TODO: This kind of configuration should be more "automatically".*/
#define KGPU_BUF_SIZE (128*128*128)

#define KGPU_MMAP_SIZE KGPU_BUF_SIZE

#define KGPU_DEV_NAME "kgpu"

/* ioctl */
#include <linux/ioctl.h>

/**
* @def KGPU_IOC_MAGIC
* This character is used to identify the device driver. To make the Kernel
* understand Kgpu as a new device driver, it must to be registered in the
* IOCTL table. The character 'g' is not set by default, because of this we
* use it.
*/
#define KGPU_IOC_MAGIC 'g'

/**
* @def KGPU_IOC_SET_GPU_BUFS
* @see http://www.cs.fsu.edu/~baker/devices/lxr/http/source/linux/Documentation/ioctl/ioctl-number.txt
* First parameter is the identify, second is the sequence number, and the
* last one is the type of data.
*/
#define KGPU_IOC_SET_GPU_BUFS \
  _IOW(KGPU_IOC_MAGIC, 1, struct kgpu_gpu_mem_info[KGPU_BUF_NR])

#define KGPU_IOC_GET_GPU_BUFS \
  _IOR(KGPU_IOC_MAGIC, 2, struct kgpu_gpu_mem_info[KGPU_BUF_NR])

#define KGPU_IOC_SET_STOP _IO(KGPU_IOC_MAGIC, 3)
#define KGPU_IOC_GET_REQS _IOR(KGPU_IOC_MAGIC, 4, 

#define KGPU_IOC_MAXNR 4

#include "kgpu_log.h"

#endif
