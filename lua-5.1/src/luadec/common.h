#pragma once

#ifndef LUADEC_COMMON_H
#define LUADEC_COMMON_H

#ifdef _DEBUG

#if defined MEMCHECK_VLD
	#include <wchar.h>
	#include <vld.h>
#elif defined MEMCHECK_MEMWATCH
	#define MEMWATCH
	#define MW_STDIO
	#include <string.h>
	#include "memwatch.h"
#endif

#endif

/**
*** we output string in ASCII by default
*** if you want use another character set, please define STRING_XXX macro as below
***    make STRING_CODING=STRING_GBK
***    make STRING_CODING=STRING_ASCII
***    or add marco define to project config in IDE
***    or uncommet one line below
*** ONLY ONE STRING_XXX is allowed
**/
//#define STRING_ASCII
//#define STRING_GB2312
//#define STRING_GBK
//#define STRING_GB18030
//#define STRING_BIG5
//#define STRING_UTF8

#endif