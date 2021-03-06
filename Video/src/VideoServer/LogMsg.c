/*
 * Project Name MFC DRIVER 
 * Copyright  2007 Samsung Electronics Co, Ltd. All Rights Reserved. 
 *
 * This software is the confidential and proprietary information
 * of Samsung Electronics  ("Confidential Information").   
 * you shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung Electronics 
 *
 * This source file is for printing the driver's log messages.
 *
 * @name MFC DRIVER MODULE Module (log_msg.c)
 * @author Jiun, Yu(jiun.yu@samsung.com)
 * @date 03-28-07
 */

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "LogMsg.h"

//#define DEBUG

static const LOG_LEVEL log_level = LOG_TRACE;

static const char *modulename = "APPLICATIONS";

static const char *level_str[] = {"TRACE", "WARNING", "ERROR"};

void LOG_MSG(LOG_LEVEL level, const char *func_name, const char *msg, ...)
{
	
	char buf[256];
	va_list argptr;

	if (level < log_level)
		return;

	sprintf(buf, "[%s: %s] %s: ", modulename, level_str[level], func_name);

	va_start(argptr, msg);
	vsprintf(buf + strlen(buf), msg, argptr);

	if(level == LOG_TRACE){
	#ifdef DEBUG
		printf(buf);
	#endif
	} else {
		printf(buf);
	}
	
	va_end(argptr);
}

