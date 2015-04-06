#ifndef _KERNEL_CODE_H_
#define _KERNEL_CODE_H_


/* the NR will not be used */
#define KGPU_BUF_NR 1
// TODO: This kind of configuration should be more "automatically".
#define KGPU_BUF_SIZE (64*64)

#define KGPU_MMAP_SIZE KGPU_BUF_SIZE

#define KGPU_DEV_NAME "kgpu"

/* ioctl */
#include <linux/ioctl.h>
// TODO: Understand and comment all of this defines.
#define KGPU_IOC_MAGIC 'g'

#define KGPU_IOC_SET_GPU_BUFS \
  _IOW(KGPU_IOC_MAGIC, 1, struct kgpu_gpu_mem_info[KGPU_BUF_NR])

#define KGPU_IOC_GET_GPU_BUFS \
  _IOR(KGPU_IOC_MAGIC, 2, struct kgpu_gpu_mem_info[KGPU_BUF_NR])

#define KGPU_IOC_SET_STOP _IO(KGPU_IOC_MAGIC, 3)
#define KGPU_IOC_GET_REQS _IOR(KGPU_IOC_MAGIC, 4, 

#define KGPU_IOC_MAXNR 4

#include "kgpu_log.h"

#endif
