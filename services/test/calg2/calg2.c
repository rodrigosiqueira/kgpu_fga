/* This work is licensed under the terms of the GNU GPL, version 2.  See
 * the GPL-COPYING file in the top-level directory.
 *
 * Copyright (c) 2010-2011 University of Utah and the Flux Group.
 * All rights reserved.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/gfp.h>
#include <linux/kthread.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/string.h>
#include <linux/uaccess.h>
#include <asm/page.h>

#include "../../../kgpu/kgpu.h"

/* customized log function */
#define g_log(level, ...) kgpu_do_log(level, "calg2", ##__VA_ARGS__)
#define dbg(...) g_log(KGPU_LOG_DEBUG, ##__VA_ARGS__)


int mycb(struct kgpu_request * pRequest)
{
    g_log(KGPU_LOG_PRINT, "REQUEST ID: %d, RESPONSE CODE: %d, %d\n", 
          pRequest->id, pRequest->errcode, ((int*)(pRequest->kdata))[0]);

    kgpu_unmap_area(TO_UL(pRequest->in));
    free_page(TO_UL(pRequest->kdata));
    kgpu_free_request(pRequest);

    return 0;
}

static int __init minit(void)
{
    struct kgpu_request * request;
    char * buf;
    unsigned long pfn;//Pointer function
    void * ga;
    
    g_log(KGPU_LOG_PRINT, "Loaded\n");

    request = kgpu_alloc_request();
    if (!request) 
    {
      g_log(KGPU_LOG_ERROR, "request null\n");
      return 0;
    }
    
    buf = (char*)__get_free_page(GFP_KERNEL);
    if (!buf)
    {
      g_log(KGPU_LOG_ERROR, "Buffer null\n");
      kgpu_free_request(request);
      return 0;
    }
    pfn = __pa(buf) >> PAGE_SHIFT;

    ga = kgpu_map_pfns(&pfn, 1);
    if (!ga)
    {
      g_log(KGPU_LOG_ERROR, "mmap error\n");
      kgpu_free_request(request);
      free_page(TO_UL(buf));
      return 0;
    }

    request->in = ga;
    request->insize = 1024;
    request->out = ga; /*+1024;*/
    request->outsize = 1024;
    request->udata = ga;
    request->udatasize = 1024;
    strcpy(request->service_name, "test_service");
    request->callback = mycb;

    request->kdata = buf;

    ((int*)(buf))[0] = 100;

    kgpu_call_async(request);

    return 0;
}

static void __exit mexit(void)
{
    g_log(KGPU_LOG_PRINT, "unload\n");
}

module_init(minit);
module_exit(mexit);

MODULE_LICENSE("GPL");
