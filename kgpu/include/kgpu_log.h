/*
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the GPL-COPYING file in the top-level directory.
 *
 * Copyright (c) 2010-2011 University of Utah and the Flux Group.
 * All rights reserved.
 *
 * Log functions
 *
 */
/**
* @file kgpu_log.h
* @brief Log handlers
*/
#ifndef __KGPU_LOG_H__
#define __KGPU_LOG_H__

#define KGPU_LOG_INFO  1
#define KGPU_LOG_DEBUG 2
#define KGPU_LOG_ALERT 3
#define KGPU_LOG_ERROR 4
#define KGPU_LOG_PRINT 5

/**
* @param pLevel Log level can be: info, debug, alert, error, and print.
* @param pModule Module Module whose log was called.
* @param pFilename Filename File name.
* @param pLineno lineno Line number.
* @param pFunction Function name.
* @param pFormat
* @brief
*/
extern void kgpu_generic_log(int pLevel, const char * pModule,
                              const char * pFilename, int pLineno,
                              const char * pFunction, const char * pFormat,
                              ...);

extern int kgpu_log_level;

#ifdef __KGPU_LOG_FULL_FILE_PATH__
  #define __FILE_NAME__ __FILE__
#else

  #ifdef __KERNEL__
    #include <linux/string.h>
  #else
    #include <string.h>
  #endif

  #define __FILE_NAME__   \
    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

/**
* TODO: Use correctly doxygen.
* @brief Print log based on level.
* This macro receives log level and module name. Additionally the file name,
*   line, function name, and other arguments are printed together.
*/
#define kgpu_do_log(level, module, ...) \
    kgpu_generic_log(level, module,     \
		     __FILE_NAME__, __LINE__, __func__, ##__VA_ARGS__)

#endif
