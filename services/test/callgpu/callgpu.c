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

#include "../../../kgpu/include/kgpu.h"

/* customized log function */
#define g_log(level, ...) kgpu_do_log(level, "calg2", ##__VA_ARGS__)
#define dbg(...) g_log(KGPU_LOG_DEBUG, ##__VA_ARGS__)

int mycb(struct kgpu_request * request)
{
  g_log(KGPU_LOG_PRINT, "REQ ID: %d, RESP CODE: %d\n", 
        request->id, request->errcode);

  kgpu_vfree(request->in);
  kgpu_free_request(request);

  return 0;
}

static int __init minit(void)
{
  struct kgpu_request * request;

  g_log(KGPU_LOG_PRINT, "loaded\n");

  request = kgpu_alloc_request();
  if (!request) 
  {
    g_log(KGPU_LOG_ERROR, "request null\n");
    return 0;
  }

  request->in = kgpu_vmalloc(1024*2);
  if (!request->in) 
  {
    g_log(KGPU_LOG_ERROR, "callgpu out of memory\n");
    kgpu_free_request(request);
    return 0;
  }

  request->insize = 1024;
  request->out = (void*) ((unsigned long)(request->in) + 1024);
  request->outsize = 1024;
  strcpy(request->service_name, "nonexist service");
  request->callback = mycb;

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
