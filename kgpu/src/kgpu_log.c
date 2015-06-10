/*
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the GPL-COPYING file in the top-level directory.
 *
 * Copyright (c) 2010-2011 University of Utah and the Flux Group.
 * All rights reserved.
 *
 * Log functions used by both kernel and user space.
 */
#include "kgpu.h"

#ifndef __KERNEL__

#include <stdio.h>
#include <stdarg.h>

// TODO: NOT GOOD!
#define printk printf
#define vprintk vprintf

#else

#include <linux/kernel.h>
#include <linux/module.h>

#endif /* __KERNEL__ */

// Configure log level.
// TODO: Try to put all this "setup" stuff on the same place.
#ifdef __KGPU_LOG_LEVEL__
  int kgpu_log_level = __KGPU_LOG_LEVEL__;
#else
  int kgpu_log_level = KGPU_LOG_DEBUG;
#endif

void kgpu_generic_log(int pLevel, const char * pModule,
                              const char * pFilename, int pLineno,
                              const char * pFunction, const char * pFormat,
                              ...)
{
  va_list args;
    
  if (pLevel < kgpu_log_level)
    return;
    
  switch(pLevel)
  {
    case KGPU_LOG_INFO:
      printk("[%s] %s::%d %s() INFO: ", pModule, pFilename,
                                        pLineno, pFunction);
      break;
    case KGPU_LOG_DEBUG:
      printk("[%s] %s::%d %s() DEBUG: ", pModule, pFilename,
                                         pLineno, pFunction);
      break;
    case KGPU_LOG_ALERT:
      printk("[%s] %s::%d %s() ALERT: ", pModule, pFilename,
                                         pLineno, pFunction);
      break;
    case KGPU_LOG_ERROR:
      printk("[%s] %s::%d %s() ERROR: ", pModule, pFilename,
                                         pLineno, pFunction);
      break;
    case KGPU_LOG_PRINT:
      printk("[%s] %s::%d %s(): ", pModule, pFilename,
                                   pLineno, pFunction);
      break;
    default:
      break;
  }
 
  va_start(args, pFormat);	
  vprintk(pFormat, args);
  va_end(args);
}

#ifdef __KERNEL__

EXPORT_SYMBOL_GPL(kgpu_generic_log);
EXPORT_SYMBOL_GPL(kgpu_log_level);

#endif /* __KERNEL__ */
