// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LIBDS_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LIBDS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LIBDS_EXPORTS
#define LIBDS_API __declspec(dllexport)
#else
#define LIBDS_API __declspec(dllimport)
#endif

#include "WMEncoder.h"
#include "RawAviEncoder.h"

extern "C" LIBDS_API struct DShow::IVideoEncoder* init_instance(int encoder);

extern "C" LIBDS_API struct DShow::IVideoEncoder* init_WindowsMediaEncoder9(void);
extern "C" LIBDS_API struct DShow::IVideoEncoder* init_RawVideoEncoder(void);

extern "C" LIBDS_API void release_instance(struct DShow::IVideoEncoder* inst, int type);