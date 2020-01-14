#include <windows.h>
#include <stdio.h>
#include <CL/cl.h>

#include "OclInfoSelectDialog_Win/OclInfoSelectDialog_Win.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int iRet;
	char cInfo[256];
	cl_platform_id Platform = NULL;
	cl_device_id Device = NULL;

	iRet = OclInfoSelectDialog(&Platform, &Device);

	printf("OclInfoSelectDialog returned: %d\n", iRet);
	if(!iRet){
		if(clGetPlatformInfo(Platform, CL_PLATFORM_NAME, sizeof(cInfo), cInfo, NULL) == CL_SUCCESS){
			puts(cInfo);
		}
		if(clGetDeviceInfo(Device, CL_DEVICE_NAME, sizeof(cInfo), cInfo, NULL) == CL_SUCCESS){
			puts(cInfo);
		}
	}
	else if(iRet < 0){
		switch(iRet){
		case OCL_RET_CANCEL:
			puts("User clicked Cancel button.");
			break;
		case OCL_RET_CLOSE:
			puts("User closed the dialog.");
			break;
		case OCL_NO_OCL_PLAT_FOUND:
			puts("No OpenCL platform found.");
			break;
		default:
			puts("Should not get here.");
			break;
		}
	}
	else{
		printf("OclInfoSelectDialog failed. (Line: %d)\n", iRet);
	}

	return 0;
}
