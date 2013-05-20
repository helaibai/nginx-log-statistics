#ifndef __DEBUG_H__
#define __DEBUG_H__
#ifdef DEBUG_ERR
#define DBG_ERR(fmt, args...) fprintf(stderr, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)
#else
#define DBG_ERR(fmt, args...)   do {  } while(0)
#endif

#ifdef DEBUG_INFO
#define DBG_INFO(fmt, args...) fprintf(stdout, "%s:%-5d: " fmt, __FUNCTION__, __LINE__, ## args)
#else
#define DBG_INFO(fmt, args...)   do {  } while(0)
#endif

#endif//__DEBUG_H__
