#ifndef FGR_COMMON_H_
#define FGR_COMMON_H_

#if defined (_MSC_VER) || defined (_WIN32) || defined (_WIN64)
#ifdef FGR_EXPORTS
#define  FGR_API __declspec(dllexport)
#else
#define  FGR_API __declspec(dllimport)
#endif 
#else // defined (windows)
#define FGR_API
#endif

#endif