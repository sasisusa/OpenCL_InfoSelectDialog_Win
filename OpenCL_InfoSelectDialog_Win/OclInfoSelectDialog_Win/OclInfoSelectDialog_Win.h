#ifndef OCLINFOSELECTDIALOG_WIN_H_
#define OCLINFOSELECTDIALOG_WIN_H_

//Libraries: gdi32, comctl32, opencl
//Linker flags (gcc): -mwindows

#if defined(_WIN32) || defined(_WIN64)


#if (defined(WINVER) && (WINVER < _WIN32_WINNT_WINXP)) || \
	(defined(_WIN32_WINNT) && (_WIN32_WINNT < _WIN32_WINNT_WINXP))
#error Windows version to old
#endif

#include <CL/cl.h>

#define OCL_RET_CANCEL -1
#define OCL_RET_CLOSE -2
#define OCL_NO_OCL_PLAT_FOUND -3


////////////////////////////////////////////////////////////////////////////
//	OclInfoSelectDialog
//
//	if pClPlatform and pClDevice = NULL -> Info-Box, else Select-Box
//
//  Returns:
//		> 0		on failure
//		0		on success
//		< 0:	OCL_RET_CANCEL		Cancel-button
//				OCL_RET_CLOSE		Alt F4, Close Window, Menu->Exit
//				OCL_NO_OCL_PLAT_FOUND		No OpenCL platform found
//
int OclInfoSelectDialog(cl_platform_id *, cl_device_id *);


#endif

#endif /* OCLINFOSELECTDIALOG_WIN_H_ */
