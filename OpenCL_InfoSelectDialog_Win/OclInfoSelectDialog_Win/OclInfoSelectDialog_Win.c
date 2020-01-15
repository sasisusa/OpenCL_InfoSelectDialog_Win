#if defined(_WIN32) || defined(_WIN64)

#if !(defined(WINVER) && defined(_WIN32_WINNT))
	#include <sdkddkver.h>
	#undef WINVER
	#define WINVER _WIN32_WINNT_VISTA
	#undef _WIN32_WINNT
	#define _WIN32_WINNT _WIN32_WINNT_VISTA
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <richedit.h>

#include <stdlib.h>
#include <stdio.h>

#include "../OclInfoSelectDialog_Win/OclInfoSelectDialog_Win.h"


#define ISWIN_WIDTH 488
#define ISWIN_HEIGHT 512

#define MAX_PLA_DEV_STRING_LEN 128
#define MAX_LIST_INFO_STRING_LEN 128

enum OCL_INFO_DATATYPE{
	DTID_STRING, DTID_DEV_TYPE, DTID_UINT, DTID_ULONG, DTID_SIZE, DTID_SIZE_N_DIM, DTID_LOC_MEM_TYPE, DTID_SEP_LIST_STRING, DTID_BOOL
};

typedef struct OCLDEVIDtag{
	unsigned int uiNumDev;
	cl_device_id *pDevice;
}OCLDEVID;

typedef struct OCLPLATNAMEtag{
	unsigned int uiNumPlat;
	cl_platform_id *pPlatform;
	OCLDEVID *pDevId;
}OCLPLATDEVID;

typedef struct PLATFORMINFOREQUESTtag{
	cl_platform_info PlatformInfo;
	enum OCL_INFO_DATATYPE InfoType;
}PLATFORMINFOREQUEST;

typedef struct DEVICEINFOREQUESTtag{
	cl_device_info DeviceInfo;
	enum OCL_INFO_DATATYPE InfoType;
}DEVICEINFOREQUEST;


#define OCL_SYSTEM_INFOS 1
#define OCL_SELECT_PLATFORM 2
#define OCL_SELECT_DEVICE 3

#define OCL_INVALIDE_NUM 0x0100

#define TREE_ONLY_VALID_PLATFORM_INDEX 1
#define TREE_VALID_DEVICE_INDEX 2

#define ID_OK 40000
#define ID_CANCEL 40001
#define ID_TREEVIEW 40002
#define ID_LISTVIEW 40003

#define ID_FILE_REFRESH 40004
#define ID_FILE_EXIT 40005
#define ID_HELP_ABOUT 40006
#define ID_SHOW_ALL_OCL 40007

typedef struct INOUTOCLPLATDEVtag{
	OCLPLATDEVID InPlatDev;
	unsigned short uSelPlatNum;
	unsigned short uSelDevNum;
	int iRequest;
	int iRet;
}INOUTOCLPLATDEV;


const char *OclPlatformInfoToString(cl_platform_info clPlatInfo)
{
	const char *sRet = NULL;

	switch(clPlatInfo){
	case CL_PLATFORM_PROFILE:
		sRet = "Profile";
		break;
	case CL_PLATFORM_VERSION:
		sRet = "Version";
		break;
	case CL_PLATFORM_NAME:
		sRet = "Name";
		break;
	case CL_PLATFORM_VENDOR:
		sRet = "Vendor";
		break;
	case CL_PLATFORM_EXTENSIONS:
		sRet = "Extensions";
		break;
	}

	return sRet;
}


const char *OclGetDevTypeString(cl_device_type *pclDevType)
{
	const char *sRet = NULL;

	if(pclDevType){
		switch(*pclDevType){
		case CL_DEVICE_TYPE_DEFAULT:
			sRet = "DEFAULT";
			break;
		case CL_DEVICE_TYPE_CPU:
			sRet = "CPU";
			break;
		case CL_DEVICE_TYPE_GPU:
			sRet = "GPU";
			break;
		case CL_DEVICE_TYPE_ACCELERATOR:
			sRet = "ACCELERATOR";
			break;
		case CL_DEVICE_TYPE_CUSTOM:
			sRet = "CUSTOM";
			break;
		}
	}

return sRet;
}


const char *OclDevieInfoToString(cl_device_info clDeviceInfo)
{
	const char *sRet = NULL;

	switch(clDeviceInfo){
	case CL_DEVICE_TYPE:
		sRet = "Device-type";
		break;
	case CL_DEVICE_NAME:
		sRet = "Name";
		break;
	case CL_DEVICE_VERSION:
		sRet = "Version";
		break;
	case CL_DEVICE_OPENCL_C_VERSION:
		sRet = "OpenCL C version";
		break;
	case CL_DRIVER_VERSION:
		sRet = "Driver-version";
		break;
	case CL_DEVICE_AVAILABLE:
		sRet = "Device available";
		break;
	case CL_DEVICE_COMPILER_AVAILABLE:
		sRet = "Compiler available";
		break;
	case CL_DEVICE_ADDRESS_BITS:
		sRet = "Address-space [bit]";
		break;
	case CL_DEVICE_MAX_COMPUTE_UNITS:
		sRet = "Max compute-units";
		break;
	case CL_DEVICE_MAX_CLOCK_FREQUENCY:
		sRet = "Max frequency [MHz]";
		break;
	case CL_DEVICE_LOCAL_MEM_SIZE:
		sRet = "Local memory size [bytes]";
		break;
	case CL_DEVICE_LOCAL_MEM_TYPE:
		sRet = "Local memory type";
		break;
	case CL_DEVICE_GLOBAL_MEM_SIZE:
		sRet = "Global memory size [bytes]";
		break;
	case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
		sRet = "Global memory cache size [bytes]";
		break;
	case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
		sRet = "Max memory allocation [bytes]";
		break;
	case CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:
		sRet = "Max work-item dimension";
		break;
	case CL_DEVICE_MAX_WORK_ITEM_SIZES:
		sRet = "Max work-item size";
		break;
	case CL_DEVICE_MAX_WORK_GROUP_SIZE:
		sRet = "Max work-group size";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:
		sRet = "Preferred vector width: CHAR";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:
		sRet = "Preferred vector width: SHORT";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:
		sRet = "Preferred vector width: INT";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:
		sRet = "Preferred vector width: LONG";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:
		sRet = "Preferred vector width: FLOAT";
		break;
	case CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:
		sRet = "Preferred vector width: DOUBLE";
		break;
	case CL_DEVICE_EXTENSIONS:
		sRet = "Extensions";
		break;
	case CL_DEVICE_PROFILING_TIMER_RESOLUTION:
		sRet = "Profiling timer resolution [ns]";
		break;
	}

	return sRet;
}


static int OclFreePlatformsAndDevices(OCLPLATDEVID *pOclPlatDev)
{
	HANDLE hHeap;
	unsigned int ui;
	BOOL bSuc;

	hHeap = GetProcessHeap();
	if(pOclPlatDev && hHeap){
		bSuc = 1;
		if(pOclPlatDev->uiNumPlat){
			for(ui=0; ui<pOclPlatDev->uiNumPlat; ++ui){
				if(pOclPlatDev->pDevId[ui].pDevice){
					bSuc &= HeapFree(hHeap, 0, pOclPlatDev->pDevId[ui].pDevice);
					pOclPlatDev->pDevId[ui].pDevice = NULL;
				}
			}
			if(pOclPlatDev->pDevId){
				bSuc &= HeapFree(hHeap, 0, pOclPlatDev->pDevId);
			}
			if(pOclPlatDev->pPlatform){
				bSuc &= HeapFree(hHeap, 0, pOclPlatDev->pPlatform);
			}
		}
		if(!bSuc){
			return 1;
		}
		ZeroMemory(pOclPlatDev, sizeof(*pOclPlatDev));
		return 0;
	}

	return 1;
}


static int OclGetPlatformsAndDevices(OCLPLATDEVID *pOclPlatDev)
{
	HANDLE hHeap;
	cl_uint NumPlatforms, NumDevice;
	unsigned int ui;

	if(!pOclPlatDev){
		return 1;
	}

	ZeroMemory(pOclPlatDev, sizeof(*pOclPlatDev));
	NumPlatforms = 0;
	hHeap = GetProcessHeap();
	if(hHeap && clGetPlatformIDs(0, NULL, &NumPlatforms) == CL_SUCCESS){
		if(!NumPlatforms){
			return -1;
		}
		pOclPlatDev->uiNumPlat = NumPlatforms;
		pOclPlatDev->pPlatform = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, NumPlatforms * sizeof(cl_platform_id));
		pOclPlatDev->pDevId = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, NumPlatforms * sizeof(OCLDEVID));

		if(!(pOclPlatDev->pPlatform && pOclPlatDev->pDevId)){
			OclFreePlatformsAndDevices(pOclPlatDev);
			return 1;
		}

		clGetPlatformIDs(NumPlatforms, pOclPlatDev->pPlatform, NULL);

		for(ui=0; ui<NumPlatforms; ++ui){
			NumDevice = 0;
			if(clGetDeviceIDs(pOclPlatDev->pPlatform[ui], CL_DEVICE_TYPE_ALL, 0, NULL, &NumDevice) == CL_SUCCESS){
				pOclPlatDev->pDevId[ui].uiNumDev = NumDevice;
				if(NumDevice){
					pOclPlatDev->pDevId[ui].pDevice = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, NumDevice * sizeof(cl_device_id));
					if(!pOclPlatDev->pDevId[ui].pDevice){
						OclFreePlatformsAndDevices(pOclPlatDev);
						return 1;
					}
					clGetDeviceIDs(pOclPlatDev->pPlatform[ui], CL_DEVICE_TYPE_ALL, NumDevice, pOclPlatDev->pDevId[ui].pDevice, NULL);
				}
			}
			else{
				OclFreePlatformsAndDevices(pOclPlatDev);
				return 1;
			}
		}
		return 0;
	}

	return 1;
}


static HTREEITEM AddItemToTree(HWND hwndTV, char *sItem, size_t zSize, LPARAM lParam, HTREEITEM hParent, HTREEITEM hBrother)
{
	TVITEM tvi = {0};
	TVINSERTSTRUCT tvins = {0};

	tvi.mask = TVIF_TEXT | TVIF_PARAM;
	tvi.pszText = sItem;
	tvi.cchTextMax = zSize;
	tvi.lParam = lParam;
	tvins.item = tvi;

	if(hBrother){
		tvins.hInsertAfter = hBrother;
	}
	else{
		tvins.hInsertAfter = TVI_FIRST;
	}

	if(hParent) {
		tvins.hParent = hParent;
	}
	else{
		tvins.hParent = TVI_ROOT;
	}

return TreeView_InsertItem(hwndTV, &tvins);
}


static int FillTree(HWND hWndTreeView, OCLPLATDEVID *pInPlatDev)
{
	unsigned int uiPlat, uiDev;
	size_t zSize;
	HTREEITEM hParent, hBrother;
	char sTmp[MAX_PLA_DEV_STRING_LEN];

	if(!(hWndTreeView && pInPlatDev)){
		return 1;
	}

	hParent = NULL;
	for(uiPlat=0; uiPlat<pInPlatDev->uiNumPlat; ++uiPlat){
		if(clGetPlatformInfo(pInPlatDev->pPlatform[uiPlat], CL_PLATFORM_NAME, sizeof(sTmp)/sizeof(*sTmp), sTmp, &zSize) == CL_SUCCESS){
			hParent = AddItemToTree(hWndTreeView, sTmp, zSize, MAKELPARAM(TREE_ONLY_VALID_PLATFORM_INDEX, MAKEWORD(uiPlat, 0)), NULL, hParent);
			if(!hParent){
				return 1;
			}
			hBrother = NULL;
			for(uiDev=0; uiDev<pInPlatDev->pDevId[uiPlat].uiNumDev; ++uiDev){
				if(clGetDeviceInfo(pInPlatDev->pDevId[uiPlat].pDevice[uiDev], CL_DEVICE_NAME, sizeof(sTmp)/sizeof(*sTmp), sTmp, &zSize) == CL_SUCCESS){
					hBrother = AddItemToTree(hWndTreeView, sTmp, zSize, MAKELPARAM(TREE_VALID_DEVICE_INDEX, MAKEWORD(uiPlat, uiDev)), hParent, hBrother);
					if(!hBrother){
						return 1;
					}
				}
				else{
					return 1;
				}
			}
		}
		else{
			return 1;
		}
	}

	return 0;
}


static int SetSelOutAndFillList(INOUTOCLPLATDEV *pInOutPlatDev, HWND hTreeView, NMTREEVIEW *pnmTreeView, HWND hWndListView)
{
	TVITEM tvit;

	if(!(pInOutPlatDev && hTreeView && pnmTreeView && hWndListView)){
		return 1;
	}

	ZeroMemory(&tvit, sizeof(tvit));
	tvit.mask = TVIF_PARAM;
	tvit.hItem = pnmTreeView->itemNew.hItem;

	if(TreeView_GetItem(hTreeView, &tvit)){
		unsigned int ui, uiN, uiOffset;
		size_t zSize, *zpSize;
		void *pInfo;
		HANDLE hHeap;
		LVITEM lvItem;
		LVCOLUMN lvCol;
		char sInfo[MAX_LIST_INFO_STRING_LEN];

		ZeroMemory(&lvCol, sizeof(lvCol));
		lvCol.mask = LVCF_TEXT;
		ZeroMemory(&lvItem, sizeof(lvItem));
		uiOffset = 0;
		pInOutPlatDev->uSelPlatNum = OCL_INVALIDE_NUM;
		pInOutPlatDev->uSelDevNum = OCL_INVALIDE_NUM;
		hHeap = GetProcessHeap();

		if(!(ListView_DeleteAllItems(hWndListView) && hHeap)){
			return 1;
		}

		if(LOWORD(tvit.lParam) == TREE_ONLY_VALID_PLATFORM_INDEX){
			const PLATFORMINFOREQUEST PlatformInfo[] = {
				{CL_PLATFORM_VENDOR, DTID_STRING},
				{CL_PLATFORM_NAME, DTID_STRING},
				{CL_PLATFORM_VERSION, DTID_STRING},
				{CL_PLATFORM_PROFILE, DTID_STRING},
				{CL_PLATFORM_EXTENSIONS, DTID_SEP_LIST_STRING}
			};
			const unsigned int uiNReq = sizeof(PlatformInfo)/sizeof(PlatformInfo[0]);
			cl_platform_id clPlatId;

			clPlatId = pInOutPlatDev->InPlatDev.pPlatform[LOBYTE(HIWORD(tvit.lParam))];
			pInOutPlatDev->uSelPlatNum = LOBYTE(HIWORD(tvit.lParam));

			lvCol.pszText = "Platform info";
			ListView_SetColumn(hWndListView, 0, &lvCol);

			for(ui=0; ui<uiNReq; ++ui){
				if(clGetPlatformInfo(clPlatId, PlatformInfo[ui].PlatformInfo, 0, NULL, &zSize) == CL_SUCCESS){
					pInfo = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, zSize);
					if(!pInfo){
						return 1;
					}
					clGetPlatformInfo(clPlatId, PlatformInfo[ui].PlatformInfo, zSize, pInfo, NULL);

					switch(PlatformInfo[ui].InfoType){
					case DTID_STRING:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", (char*)pInfo);
						break;
					case DTID_SEP_LIST_STRING:{
						unsigned int uiS;
						char *sRun = (char*)pInfo;
						char *sTmpInfoType = (char*)OclPlatformInfoToString(PlatformInfo[ui].PlatformInfo);

						while(*sRun != '\0'){
							uiS = 0;
							while(*sRun != ' ' && *sRun != ';' && *sRun != '\0' && uiS < sizeof(sInfo)/sizeof(*sInfo)-1){
								sInfo[uiS++] = *sRun++;
							}
							sInfo[uiS] = '\0';

							if(*sRun != '\0'){
								do{
									++sRun;
								}while(*sRun == ' ' || *sRun == ';');
							}

							lvItem.iItem = ui + uiOffset;
							if(ListView_InsertItem(hWndListView, &lvItem) == -1){
								HeapFree(hHeap, 0, pInfo);
								return 1;
							}
							ListView_SetItemText(hWndListView, ui + uiOffset, 0, sTmpInfoType);
							ListView_SetItemText(hWndListView, ui + uiOffset, 1, sInfo);
							sTmpInfoType = "";
							++uiOffset;
						}
						--uiOffset;
						if(!HeapFree(hHeap, 0, pInfo)){
							return 1;
						}
						}continue;
					default:
						sInfo[0] = '\0';
						break;
					}

					if(!HeapFree(hHeap, 0, pInfo)){
						return 1;
					}
					lvItem.iItem = ui + uiOffset;
					if(ListView_InsertItem(hWndListView, &lvItem) == -1){
						return 1;
					}
					ListView_SetItemText(hWndListView, ui + uiOffset, 0, (char*)OclPlatformInfoToString(PlatformInfo[ui].PlatformInfo));
					ListView_SetItemText(hWndListView, ui + uiOffset, 1, sInfo);
				}
			}
		}
		else if(LOWORD(tvit.lParam) == TREE_VALID_DEVICE_INDEX){
			const DEVICEINFOREQUEST DeviceInfo[] = {
				{CL_DEVICE_TYPE, DTID_DEV_TYPE},
				{CL_DEVICE_NAME, DTID_STRING},
				{CL_DEVICE_VERSION, DTID_STRING},
				{CL_DEVICE_OPENCL_C_VERSION, DTID_STRING},
				{CL_DRIVER_VERSION, DTID_STRING},
				{CL_DEVICE_AVAILABLE, DTID_BOOL},
				{CL_DEVICE_COMPILER_AVAILABLE, DTID_BOOL},
				{CL_DEVICE_ADDRESS_BITS, DTID_UINT},
				{CL_DEVICE_MAX_COMPUTE_UNITS, DTID_UINT},
				{CL_DEVICE_MAX_CLOCK_FREQUENCY, DTID_UINT},
				{CL_DEVICE_LOCAL_MEM_SIZE, DTID_ULONG},
				{CL_DEVICE_LOCAL_MEM_TYPE, DTID_LOC_MEM_TYPE},
				{CL_DEVICE_GLOBAL_MEM_SIZE, DTID_ULONG},
				{CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, DTID_ULONG},
				{CL_DEVICE_MAX_MEM_ALLOC_SIZE, DTID_ULONG},
				{CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, DTID_UINT},
				{CL_DEVICE_MAX_WORK_ITEM_SIZES, DTID_SIZE_N_DIM},
				{CL_DEVICE_MAX_WORK_GROUP_SIZE, DTID_SIZE},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, DTID_UINT},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, DTID_UINT},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, DTID_UINT},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, DTID_UINT},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, DTID_UINT},
				{CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, DTID_UINT},
				{CL_DEVICE_EXTENSIONS, DTID_SEP_LIST_STRING},
				{CL_DEVICE_PROFILING_TIMER_RESOLUTION, DTID_SIZE}
			};
			const unsigned int uiNReq = sizeof(DeviceInfo)/sizeof(DeviceInfo[0]);
			cl_device_id clDevId;
			int i;

			pInOutPlatDev->uSelPlatNum = LOBYTE(HIWORD(tvit.lParam));
			pInOutPlatDev->uSelDevNum = HIBYTE(HIWORD(tvit.lParam));
			clDevId = pInOutPlatDev->InPlatDev.pDevId[LOBYTE(HIWORD(tvit.lParam))].pDevice[HIBYTE(HIWORD(tvit.lParam))];

			lvCol.pszText = "Device info";
			ListView_SetColumn(hWndListView, 0, &lvCol);

			for(ui=0; ui<uiNReq; ++ui){
				if(clGetDeviceInfo(clDevId, DeviceInfo[ui].DeviceInfo, 0, NULL, &zSize) == CL_SUCCESS){
					pInfo = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, zSize);
					if(!pInfo){
						return 1;
					}
					clGetDeviceInfo(clDevId, DeviceInfo[ui].DeviceInfo, zSize, pInfo, NULL);

					switch(DeviceInfo[ui].InfoType){
					case DTID_DEV_TYPE:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", OclGetDevTypeString((cl_device_type*)pInfo));
						break;
					case DTID_STRING:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", (char*)pInfo);
						break;
					case DTID_UINT:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%u", *(cl_uint*)pInfo);
						break;
					case DTID_ULONG:
						switch(DeviceInfo[ui].DeviceInfo){
						case CL_DEVICE_LOCAL_MEM_SIZE:
						case CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%I64u (%I64u K)", *(cl_ulong*)pInfo, (*(cl_ulong*)pInfo)>>10);
							break;
						case CL_DEVICE_GLOBAL_MEM_SIZE:
						case CL_DEVICE_MAX_MEM_ALLOC_SIZE:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%I64u (%I64u M)", *(cl_ulong*)pInfo, (*(cl_ulong*)pInfo)>>20);
							break;
						default:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%I64u", *(cl_ulong*)pInfo);
							break;
						}
						break;
					case DTID_SIZE:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), (sizeof(size_t)==8)?"%I64u":"%I32u", *(size_t*)pInfo);
						break;
					case DTID_SIZE_N_DIM:
						zpSize = (size_t*)pInfo;
						i = snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), (sizeof(size_t)==8)?"%I64u":"%I32u", *zpSize++);
						for(uiN = zSize/sizeof(size_t)-1; uiN>0; --uiN){
							if(i > 0 && i < sizeof(sInfo)/sizeof(*sInfo)){
								i += snprintf(&sInfo[i], sizeof(sInfo)/sizeof(*sInfo)-i, (sizeof(size_t)==8)?" x %I64u":" x %I32u", *zpSize++);
							}
						}
						break;
					case DTID_LOC_MEM_TYPE:
						switch(*(cl_device_local_mem_type*)pInfo){
						case CL_LOCAL:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", "local");
							break;
						case CL_GLOBAL:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", "global");
							break;
						case CL_NONE:
							snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", "none");
							break;
						}
						break;
					case DTID_BOOL:
						snprintf(sInfo, sizeof(sInfo)/sizeof(*sInfo), "%s", *(cl_bool*)pInfo == CL_TRUE ? "true" : "false");
						break;
					case DTID_SEP_LIST_STRING:{
						unsigned int uiS;
						char *sRun = (char*)pInfo;
						char *sTmpInfoType = (char*)OclDevieInfoToString(DeviceInfo[ui].DeviceInfo);

						while(*sRun != '\0'){
							uiS = 0;
							while(*sRun != ' ' && *sRun != ';' && *sRun != '\0' && uiS < sizeof(sInfo)/sizeof(*sInfo)-1){
								sInfo[uiS++] = *sRun++;
							}
							sInfo[uiS] = '\0';

							if(*sRun != '\0'){
								do{
									++sRun;
								}while(*sRun == ' ' || *sRun == ';');
							}

							lvItem.iItem = ui + uiOffset;
							if(ListView_InsertItem(hWndListView, &lvItem) == -1){
								HeapFree(hHeap, 0, pInfo);
								return 1;
							}
							ListView_SetItemText(hWndListView, ui + uiOffset, 0, sTmpInfoType);
							ListView_SetItemText(hWndListView, ui + uiOffset, 1, sInfo);
							sTmpInfoType = "";
							++uiOffset;
						}
						--uiOffset;
						if(!HeapFree(hHeap, 0, pInfo)){
							return 1;
						}
						}continue;
					default:
						sInfo[0] = '\0';
						break;
					}

					if(!HeapFree(hHeap, 0, pInfo)){
						return 1;
					}
					lvItem.iItem = ui + uiOffset;
					ListView_InsertItem(hWndListView, &lvItem);
					ListView_SetItemText(hWndListView, ui + uiOffset, 0, (char*)OclDevieInfoToString(DeviceInfo[ui].DeviceInfo));
					ListView_SetItemText(hWndListView, ui + uiOffset, 1, sInfo);
				}
			}
		}
	}
	else{
		return 1;
	}

	return 0;
}


static LRESULT CALLBACK SelOclPlatDevProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:{
		HWND hWndTV, hWndLV, hWndOk, hWndCancel;
		INOUTOCLPLATDEV *pInOutPlatDev;
		HFONT hFont;
		int iWidth, iHeight;
		RECT rect = {0};
		LVCOLUMN lvCol = {0};

		pInOutPlatDev = ((CREATESTRUCT*)lParam)->lpCreateParams;
		if(!pInOutPlatDev){
			DestroyWindow(hWnd);
			return -1;
		}
		pInOutPlatDev->uSelPlatNum = OCL_INVALIDE_NUM;
		pInOutPlatDev->uSelDevNum = OCL_INVALIDE_NUM;
		SetClassLongPtr(hWnd, 0, (LONG_PTR)pInOutPlatDev);
		if((INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0) != pInOutPlatDev){
			DestroyWindow(hWnd);
			return -1;
		}

		if(GetClientRect(hWnd, &rect)){	// After menu for correct values
			iWidth = rect.right - rect.left;
			iHeight = rect.bottom - rect.top;
		}
		else{
			pInOutPlatDev->iRet = __LINE__;
			DestroyWindow(hWnd);
			return -1;
		}

		hWndOk = CreateWindowEx(0, WC_BUTTON, "OK", WS_VISIBLE | WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON | BS_FLAT |
								(pInOutPlatDev->iRequest != OCL_SYSTEM_INFOS ? WS_DISABLED : 0),
								iWidth-(pInOutPlatDev->iRequest != OCL_SYSTEM_INFOS ? 184 : 90), iHeight-28,
								86, 22, hWnd, (HMENU)ID_OK, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		hWndCancel = CreateWindowEx(0, WC_BUTTON, "Cancel", WS_CHILD | WS_TABSTOP | BS_PUSHBUTTON | BS_FLAT |
									(pInOutPlatDev->iRequest == OCL_SYSTEM_INFOS ? WS_DISABLED : WS_VISIBLE),
									iWidth-90, iHeight-28, 86, 22, hWnd, (HMENU)ID_CANCEL, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		hWndTV = CreateWindowEx(0, WC_TREEVIEW, NULL,
								WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | WS_HSCROLL | WS_TABSTOP |
								TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS,
								4, 4, iWidth-8, 150, hWnd, (HMENU)ID_TREEVIEW, ((CREATESTRUCT*)lParam)->hInstance, NULL);
		hWndLV = CreateWindowEx(0 ,WC_LISTVIEW, NULL,
								WS_VISIBLE | WS_BORDER | WS_CHILD | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE |
								LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL,
								4, 158, iWidth-8, iHeight-192,
								hWnd, (HMENU)ID_LISTVIEW, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		if(!(hWndOk && hWndCancel && hWndTV && hWndLV)|| FillTree(hWndTV, &pInOutPlatDev->InPlatDev)){
			pInOutPlatDev->iRet = __LINE__;
			DestroyWindow(hWnd);
			return -1;
		}

		hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if(hFont){
			SendMessage(hWndOk, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
			SendMessage(hWndCancel, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		}

		lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.iSubItem = 0;
		lvCol.cx = 190;
		lvCol.pszText = "Info";
		ListView_InsertColumn(hWndLV, 0, &lvCol);
		lvCol.iSubItem = 1;
		lvCol.cx = 260;
		lvCol.pszText = "Value";
		ListView_InsertColumn(hWndLV, 1, &lvCol);
	}return 0;
	case WM_NOTIFY:{
		NMHDR* nmhdr = (NMHDR*)lParam;
		if(nmhdr->idFrom == ID_TREEVIEW){
			switch(nmhdr->code){
			case TVN_SELCHANGED:{
				INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
				HWND hWndOk = GetDlgItem(hWnd, ID_OK);

				if(!pInOutPlatDev){
					DestroyWindow(hWnd);
				}
				if(!hWndOk || SetSelOutAndFillList(pInOutPlatDev, nmhdr->hwndFrom, (NMTREEVIEW *)lParam, GetDlgItem(hWnd, ID_LISTVIEW))){
					pInOutPlatDev->iRet = __LINE__;
					DestroyWindow(hWnd);
					return 0;
				}
				if(pInOutPlatDev->iRequest == OCL_SELECT_DEVICE && pInOutPlatDev->uSelDevNum < OCL_INVALIDE_NUM){
					EnableWindow(hWndOk, TRUE);
				}
				else if(pInOutPlatDev->iRequest == OCL_SELECT_PLATFORM && pInOutPlatDev->uSelPlatNum < OCL_INVALIDE_NUM){
					EnableWindow(hWndOk, TRUE);
				}
				else if(pInOutPlatDev->iRequest != OCL_SYSTEM_INFOS){
					EnableWindow(hWndOk, FALSE);
				}
			}break;
			case NM_DBLCLK:{
				INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
				if(!pInOutPlatDev){
					DestroyWindow(hWnd);
				}
				if(pInOutPlatDev->iRequest == OCL_SELECT_DEVICE &&  pInOutPlatDev->uSelDevNum < OCL_INVALIDE_NUM){
					pInOutPlatDev->iRet = 0;
					DestroyWindow(hWnd);
				}
				else if(pInOutPlatDev->iRequest == OCL_SELECT_PLATFORM &&  pInOutPlatDev->uSelPlatNum < OCL_INVALIDE_NUM){
					pInOutPlatDev->iRet = 0;
					DestroyWindow(hWnd);
				}
			}break;
			}
		}
	}break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
			case ID_OK:{
				INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);

				if(!pInOutPlatDev){
					DestroyWindow(hWnd);
				}
				if(pInOutPlatDev->iRequest == OCL_SELECT_DEVICE && pInOutPlatDev->uSelDevNum < OCL_INVALIDE_NUM){
					pInOutPlatDev->iRet = 0;
					DestroyWindow(hWnd);
				}
				else if(pInOutPlatDev->iRequest == OCL_SELECT_PLATFORM && pInOutPlatDev->uSelPlatNum < OCL_INVALIDE_NUM){
					pInOutPlatDev->iRet = 0;
					DestroyWindow(hWnd);
				}
				else{	// Should not get here
					if(pInOutPlatDev->iRequest == OCL_SELECT_DEVICE){
						MessageBox(hWnd, "Please select a device.", "OpenCL device selection", MB_OK | MB_ICONINFORMATION);
					}
					else if(pInOutPlatDev->iRequest == OCL_SELECT_PLATFORM){
						MessageBox(hWnd, "Please select a platform.", "OpenCL platform selection", MB_OK | MB_ICONINFORMATION);
					}
					else if(pInOutPlatDev->iRequest == OCL_SYSTEM_INFOS){
						pInOutPlatDev->iRet = 0;
						DestroyWindow(hWnd);
					}
				}
			}break;
			case ID_HELP_ABOUT:
				MessageBox(hWnd, "Dialog box for information and selection of an OpenCL platform or device.", "About", MB_OK | MB_ICONQUESTION); //OpenCL platform and device info and select dialog box.
				return 0;
			case ID_FILE_REFRESH:{
					INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
					HWND hWndTreeView = GetDlgItem(hWnd, ID_TREEVIEW);
					HWND hWndListView = GetDlgItem(hWnd, ID_LISTVIEW);
					HWND hWndOk = GetDlgItem(hWnd, ID_OK);

					if(!pInOutPlatDev){
						DestroyWindow(hWnd);
					}
					if(!(hWndTreeView && hWndListView && hWndOk)){
						pInOutPlatDev->iRet = __LINE__;
						DestroyWindow(hWnd);
						return 0;
					}
					if(!(TreeView_DeleteAllItems(hWndTreeView) && ListView_DeleteAllItems(hWndListView))){
						pInOutPlatDev->iRet = __LINE__;
						DestroyWindow(hWnd);
						return 0;
					}
					OclFreePlatformsAndDevices(&pInOutPlatDev->InPlatDev);
					if(OclGetPlatformsAndDevices(&pInOutPlatDev->InPlatDev) || FillTree(hWndTreeView, &pInOutPlatDev->InPlatDev)){
						pInOutPlatDev->iRet = __LINE__;
						DestroyWindow(hWnd);
						return 0;
					}
					if(pInOutPlatDev->iRequest != OCL_SYSTEM_INFOS){
						EnableWindow(hWndOk, FALSE);
					}
					pInOutPlatDev->uSelPlatNum = OCL_INVALIDE_NUM;
					pInOutPlatDev->uSelDevNum = OCL_INVALIDE_NUM;

					SetFocus(GetDlgItem(hWnd, ID_TREEVIEW)?:hWnd);
				}return 0;
			case ID_FILE_EXIT:{
				INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
				if(pInOutPlatDev){
					pInOutPlatDev->iRet = OCL_RET_CLOSE;
				}
				DestroyWindow(hWnd);
				}break;
			case ID_CANCEL:{
				INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
				if(pInOutPlatDev){
					pInOutPlatDev->iRet = OCL_RET_CANCEL;
				}
				DestroyWindow(hWnd);
				}break;
		}break;
	case WM_GETMINMAXINFO:{
		MINMAXINFO *pMMI = (LPMINMAXINFO)lParam;
		RECT rect;

		SetRect(&rect, 0, 0, ISWIN_WIDTH, ISWIN_HEIGHT);
		AdjustWindowRectEx(&rect, GetWindowLongPtr(hWnd, GWL_STYLE), TRUE, GetWindowLongPtr(hWnd, GWL_EXSTYLE));
		pMMI->ptMinTrackSize.x = rect.right-rect.left;
		pMMI->ptMinTrackSize.y = rect.bottom-rect.top;
	}break;
	case WM_SIZE:{
		HWND hWndTreeView = GetDlgItem(hWnd, ID_TREEVIEW);
		HWND hWndListView = GetDlgItem(hWnd, ID_LISTVIEW);
		HWND hWndOk = GetDlgItem(hWnd, ID_OK);
		HWND hWndCancel = GetDlgItem(hWnd, ID_CANCEL);
		INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);

		if(!pInOutPlatDev){
			DestroyWindow(hWnd);
		}

		if(hWndTreeView && hWndListView && hWndOk && hWndCancel){
			MoveWindow(hWndOk, LOWORD(lParam)-(pInOutPlatDev->iRequest != OCL_SYSTEM_INFOS ? 184 : 90), HIWORD(lParam)-28, 86, 22, TRUE);
			MoveWindow(hWndCancel, LOWORD(lParam)-90, HIWORD(lParam)-28, 86, 22, TRUE);
			MoveWindow(hWndTreeView, 4, 4, LOWORD(lParam)-8, 150, TRUE);
			MoveWindow(hWndListView, 4, 158, LOWORD(lParam)-8, HIWORD(lParam)-192, TRUE);
		}

		}break;;
	case WM_CLOSE:{
		INOUTOCLPLATDEV *pInOutPlatDev = (INOUTOCLPLATDEV*)GetClassLongPtr(hWnd, 0);
		if(pInOutPlatDev){
			pInOutPlatDev->iRet = OCL_RET_CLOSE;
		}
		}break;
    case WM_DESTROY:
    	PostQuitMessage(0);
    	return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


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
int OclInfoSelectDialog(cl_platform_id *pClPlatformRet, cl_device_id *pClDeviceRet)
{
	HWND hWndD;
	HMENU hMenu, hSubFile, hSubHelp;
	char *sTitel;
	int iX, iY;
	MSG msg;
	ATOM atomClassID;
	RECT rect;
	DWORD dwStyle, dwExStyle;
	WNDCLASSEX wcex;
	INOUTOCLPLATDEV InOutPlatDev;

	ZeroMemory(&InOutPlatDev, sizeof(InOutPlatDev));
	InOutPlatDev.uSelPlatNum = OCL_INVALIDE_NUM;
	InOutPlatDev.uSelDevNum = OCL_INVALIDE_NUM;
	InOutPlatDev.iRet = 1;

	if(OclGetPlatformsAndDevices(&InOutPlatDev.InPlatDev)){
		return __LINE__;
	}

	if(!InOutPlatDev.InPlatDev.uiNumPlat){
		MessageBox(NULL, "No OpenCL platform found.", "OpenCL Info", MB_OK | MB_ICONINFORMATION);
		OclFreePlatformsAndDevices(&InOutPlatDev.InPlatDev);
		return OCL_NO_OCL_PLAT_FOUND;
	}

	InitCommonControls();

	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = SelOclPlatDevProc;
	wcex.cbClsExtra = sizeof(&InOutPlatDev);
	wcex.hInstance = NULL;	//(HINSTANCE)GetModuleHandle(NULL);
	wcex.hIcon = LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_SHARED);//LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);	//NULL
	//wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	//wcex.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wcex.lpszClassName = "oclInfoSelDialog";

	atomClassID = RegisterClassEx(&wcex);
	if(!atomClassID){
		OclFreePlatformsAndDevices(&InOutPlatDev.InPlatDev);
		return __LINE__;
	}

	if(pClDeviceRet){
		InOutPlatDev.iRequest = OCL_SELECT_DEVICE;
		sTitel = "OpenCL Select Device";
	}
	else if(pClPlatformRet){
		InOutPlatDev.iRequest = OCL_SELECT_PLATFORM;
		sTitel = "OpenCL Select Platform";
	}
	else{
		InOutPlatDev.iRequest = OCL_SYSTEM_INFOS;
		sTitel = "OpenCL Info";
	}

	hSubHelp = NULL;
	hSubFile = NULL;
	hMenu = CreateMenu();
	if(hMenu){
		hSubFile = CreatePopupMenu();
		if(hSubFile){
			AppendMenu(hSubFile, MF_STRING, ID_FILE_REFRESH, "&Refresh");
			AppendMenu(hSubFile, MF_SEPARATOR, 0, 0);
			AppendMenu(hSubFile, MF_STRING, ID_FILE_EXIT, "E&xit");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubFile, "&File");
		}
		hSubHelp = CreatePopupMenu();
		if(hSubHelp){
			AppendMenu(hSubHelp, MF_STRING, ID_HELP_ABOUT, "&About");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT_PTR)hSubHelp, "?");
		}
	}
	if(!(hMenu && hSubFile && hSubHelp)){
		UnregisterClass(MAKEINTATOM(atomClassID), wcex.hInstance);
		OclFreePlatformsAndDevices(&InOutPlatDev.InPlatDev);
		return __LINE__;
	}

	dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	//dwStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	SetRect(&rect, 0, 0, ISWIN_WIDTH, ISWIN_HEIGHT);
	AdjustWindowRectEx(&rect, dwStyle, TRUE, dwExStyle);

	iX = (GetSystemMetrics(SM_CXSCREEN)-(rect.right-rect.left))>>1;
	if(iX < 1){
		iX = CW_USEDEFAULT;
	}
	iY = (GetSystemMetrics(SM_CYSCREEN)-(rect.bottom-rect.top))>>1;
	if(iY < 1){
		iY = CW_USEDEFAULT;
	}

	hWndD = CreateWindowEx( dwExStyle, MAKEINTATOM(atomClassID), sTitel, dwStyle,
							iX, iY, rect.right-rect.left, rect.bottom-rect.top,
							NULL, hMenu, wcex.hInstance, &InOutPlatDev);

	if(!hWndD){
		UnregisterClass(MAKEINTATOM(atomClassID), wcex.hInstance);
		OclFreePlatformsAndDevices(&InOutPlatDev.InPlatDev);
		return __LINE__;
	}

	ShowWindow(hWndD, SW_SHOW);
	UpdateWindow(hWndD);
	SetForegroundWindow(hWndD);
	SetFocus(GetDlgItem(hWndD, ID_TREEVIEW)?:hWndD);


	while(GetMessage(&msg, NULL, 0, 0) > 0){
		if(!IsDialogMessage(hWndD, &msg)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	if(!UnregisterClass(MAKEINTATOM(atomClassID), wcex.hInstance)){
		InOutPlatDev.iRet = __LINE__;
	}

	if(InOutPlatDev.iRet == 0){
		if(InOutPlatDev.uSelPlatNum < OCL_INVALIDE_NUM){
			if(pClPlatformRet){
				*pClPlatformRet = InOutPlatDev.InPlatDev.pPlatform[InOutPlatDev.uSelPlatNum];
			}
			if(pClDeviceRet){
				if(InOutPlatDev.uSelDevNum < OCL_INVALIDE_NUM){
					*pClDeviceRet = InOutPlatDev.InPlatDev.pDevId[InOutPlatDev.uSelPlatNum].pDevice[InOutPlatDev.uSelDevNum];
				}
				else{
					InOutPlatDev.iRet = __LINE__;
				}
			}
		}
		else{
			InOutPlatDev.iRet = __LINE__;
		}
	}

	if(OclFreePlatformsAndDevices(&InOutPlatDev.InPlatDev)){
		InOutPlatDev.iRet = __LINE__;
	}

	return InOutPlatDev.iRet;
}

#endif
