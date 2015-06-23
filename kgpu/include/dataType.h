#ifndef _DATA_TYPE_H_
#define _DATA_TYPE_H_

#include "kgpu.h"

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

/**
* ATTENTION: THOSE STRUCTURES WILL BE DEFINED IN HELPEC.C
*/
extern struct kgpu_gpu_mem_info hostbuf;
extern struct kgpu_gpu_mem_info hostvma;

/**
* ATTENTION: THOSE STRUCTURES WILL BE DEFINED IN OPENCLOPERATIONS.C AND
*   GPUOPTS.C
*/
extern struct kgpu_gpu_mem_info devbuf;
extern struct kgpu_gpu_mem_info devbuf4vma;

#endif
