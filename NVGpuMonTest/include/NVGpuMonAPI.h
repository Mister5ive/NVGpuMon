// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 NVGPUMON_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// NVGPUMON_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef NVGPUMON_EXPORTS
#define NVGPUMON_API __declspec(dllexport)
#else
#define NVGPUMON_API __declspec(dllimport)
#endif

#pragma once  

#include"stdio.h"
#include<iostream>
#include "windows.h"  
#include"stdlib.h"
#include <assert.h>  

//! \ingroup gpupstate
#define NVAPI_MAX_GPU_UTILIZATIONS 8

#ifndef __unix
// mac os 32-bit still needs this
#if ( (defined(macintosh) && defined(__LP64__) && (__NVAPI_RESERVED0__)) || \
      (!defined(macintosh) && defined(__NVAPI_RESERVED0__)) ) 
typedef unsigned int       NvU32; /* 0 to 4294967295                         */
#else
typedef unsigned long      NvU32; /* 0 to 4294967295                         */
#endif
#else
typedef unsigned int       NvU32; /* 0 to 4294967295                         */
#endif


//! \ingroup gpupstate
//! Used in NvAPI_GPU_GetDynamicPstatesInfoEx().
typedef struct
{
	NvU32       version;        //!< Structure version
	NvU32       flags;          //!< bit 0 indicates if the dynamic Pstate is enabled or not
	struct
	{
		NvU32   bIsPresent : 1;   //!< Set if this utilization domain is present on this GPU
		NvU32   percentage;     //!< Percentage of time where the domain is considered busy in the last 1 second interval
	} utilization[NVAPI_MAX_GPU_UTILIZATIONS];
} NV_GPU_DYNAMIC_PSTATES_INFO_EX;


class NVGpuMon
{
public:
	virtual ~NVGpuMon(void){};
	NVGpuMon(void){};
	//  
	//get describing the version of the NvAPI library  
	//  
	virtual bool GetInterfaceVersionString(LPSTR strVer) = 0;
	//  
	//This function returns a struct that describes aspects of the display driver build.  
	//  
	virtual bool GetDisplayDriverVersion() = 0;
	//  
	//Get physical gpu count  
	//  
	virtual int GetPhyGpuCount() = 0;
	//  
	//  
	//  
	virtual bool GetGpuTemperature(int nIndex, int& nMinTemp, int& nCurrentTemp, int& nMaxTemp) = 0;
	//  
	//Get physical full name by index  
	//return -1:buffer is too small,0 failed  
	//  
	virtual int GetGpuName(int nIndex, LPSTR szName, int nBuffSize) = 0;

	//  
	//the GPU type (integrated or discrete)  
	//  
	//NV_SYSTEM_TYPE_GPU_UNKNOWN   0  
	//NV_SYSTEM_TYPE_IGPU   1 Integrated GPU.  
	//NV_SYSTEM_TYPE_DGPU  2 Discrete GPU.   
	//  
	virtual int GetGpuType(int nIndex) = 0;
	//  
	//Get system type(notebook GPU or a desktop GPU)  
	//  
	//NV_SYSTEM_TYPE_UNKNOWN   0  
	//NV_SYSTEM_TYPE_LAPTOP   1  
	//NV_SYSTEM_TYPE_DESKTOP   2  
	virtual int GetSysType(int nIndex) = 0;

	//  
	//the virtual size of framebuffer in KB. This includes the physical RAM plus any system RAM that has been dedicated for use by the GPU.  
	//  
	virtual int GetVirtualFrameBufferSize(int nIndex) = 0;

	//  
	//the physical size of framebuffer in KB. This does NOT include any system RAM that may be dedicated for use by the GPU  
	//  
	virtual int GetPhysicalFrameBufferSize(int nIndex) = 0;

	//  
	//the AGP aperture in megabytes.  
	//  
	virtual int GetAGPAperture(int nIndex) = 0;

	//  
	//the current AGP Rate (0 = AGP not present, 1 = 1x, 2 = 2x, etc.)  
	//  
	virtual int GetCurrentAGPRate(int nIndex) = 0;

	//  
	//Returns the ID of the bus associated with this GPU  
	//  
	virtual int GetBusId(int nIndex) = 0;

	//  
	// Returns the ID of the bus slot associated with this GPU.  
	//  
	virtual int GetBusSlotId(int nIndex) = 0;

	//  
	//returns the type of bus associated with this GPU.  
	//  
	//NVAPI_GPU_BUS_TYPE_UNDEFINED     
	//NVAPI_GPU_BUS_TYPE_PCI     
	//  NVAPI_GPU_BUS_TYPE_AGP     
	//  NVAPI_GPU_BUS_TYPE_PCI_EXPRESS     
	//  NVAPI_GPU_BUS_TYPE_FPCI    

	//  
	virtual int GetBusType(int nIndex) = 0;

	//  
	//PCI identifiers associated with this GPU.  
	//  
	virtual bool GetPCIIdentifiers(int nIndex, ULONG32&DeviceId, ULONG32 &SubSystemId, ULONG32 &RevisionId, ULONG32&ExtDeviceId) = 0;

	//  
	//the fan speed tachometer reading for the specified physical GPU  
	//  
	virtual int GetTachReading(int nIndex) = 0;

	virtual int GetGpuDynamicInfo(int index, NV_GPU_DYNAMIC_PSTATES_INFO_EX &PstatesInfo) = 0;


};





//接口

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	NVGPUMON_API NVGpuMon* NVGpuMonCreate(void);
	NVGPUMON_API void NVGpuMonDestroy(NVGpuMon*);


#ifdef __cplusplus
}
#endif // __cplusplus
