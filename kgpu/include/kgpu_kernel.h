#ifndef _KGPU_KERNEL_H_
#define _KGPU_KERNEL_H_

#include <linux/list.h>
#include <asm/page.h>

struct kgpu_request;

typedef int (*kgpu_callback)(struct kgpu_request *req);

struct kgpu_request 
{
  int id;
  void * in;
  void * out;
  void * udata;
  void * kdata;
  unsigned long insize;
  unsigned long outsize;
  unsigned long udatasize;
  unsigned long kdatasize;
  char service_name[KGPU_SERVICE_NAME_SIZE];
  kgpu_callback callback;
  int errcode;
};

extern int kgpu_call_sync (struct kgpu_request * );
extern int kgpu_call_async (struct kgpu_request * );

extern int kgpu_next_request_id (void);
extern struct kgpu_request * kgpu_alloc_request (void);
extern void kgpu_free_request (struct kgpu_request * );

extern void * kgpu_vmalloc (unsigned long nbytes);
extern void kgpu_vfree (void * p);

extern void * kgpu_map_pfns (unsigned long * pfns, int n);
extern void * kgpu_map_pages (struct page ** pages, int n);
extern void kgpu_unmap_area (unsigned long addr);
extern int kgpu_map_page (struct page *, unsigned long);
extern void kgpu_free_mmap_area (unsigned long);
extern unsigned long kgpu_alloc_mmap_area (unsigned long);

#endif
