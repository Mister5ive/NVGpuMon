
#pragma once  
#include"stdafx.h"

#ifdef _WIN64 
#pragma comment(lib,"../lib/nvapi64.lib")  
 
#elif _WIN32
#pragma comment(lib,"../lib/nvapi.lib")  
#endif  


class NVGpuMon
{
public:
	virtual ~NVGpuMon(void);
public:
	NVGpuMon(void);

public:
	//  
	//get describing the version of the NvAPI library  
	//  
	bool GetInterfaceVersionString(LPSTR strVer);
	//  
	//This function returns a struct that describes aspects of the display driver build.  
	//  
	bool GetDisplayDriverVersion();
	//  
	//Get physical gpu count  
	//  
	int GetPhyGpuCount();
	//  
	//  
	//  
	bool GetGpuTemperature(int nIndex, int& nMinTemp, int& nCurrentTemp, int& nMaxTemp);
	//  
	//Get physical full name by index  
	//return -1:buffer is too small,0 failed  
	//  
	int GetGpuName(int nIndex, LPSTR szName, int nBuffSize);

	//  
	//the GPU type (integrated or discrete)  
	//  
	//NV_SYSTEM_TYPE_GPU_UNKNOWN   0  
	//NV_SYSTEM_TYPE_IGPU   1 Integrated GPU.  
	//NV_SYSTEM_TYPE_DGPU  2 Discrete GPU.   
	//  
	int GetGpuType(int nIndex);
	//  
	//Get system type(notebook GPU or a desktop GPU)  
	//  
	//NV_SYSTEM_TYPE_UNKNOWN   0  
	//NV_SYSTEM_TYPE_LAPTOP   1  
	//NV_SYSTEM_TYPE_DESKTOP   2  
	int GetSysType(int nIndex);

	//  
	//the virtual size of framebuffer in KB. This includes the physical RAM plus any system RAM that has been dedicated for use by the GPU.  
	//  
	int GetVirtualFrameBufferSize(int nIndex);

	//  
	//the physical size of framebuffer in KB. This does NOT include any system RAM that may be dedicated for use by the GPU  
	//  
	int GetPhysicalFrameBufferSize(int nIndex);

	//  
	//the AGP aperture in megabytes.  
	//  
	int GetAGPAperture(int nIndex);

	//  
	//the current AGP Rate (0 = AGP not present, 1 = 1x, 2 = 2x, etc.)  
	//  
	int GetCurrentAGPRate(int nIndex);

	//  
	//Returns the ID of the bus associated with this GPU  
	//  
	int GetBusId(int nIndex);

	//  
	// Returns the ID of the bus slot associated with this GPU.  
	//  
	int GetBusSlotId(int nIndex);

	//  
	//returns the type of bus associated with this GPU.  
	//  
	//NVAPI_GPU_BUS_TYPE_UNDEFINED     
	//NVAPI_GPU_BUS_TYPE_PCI     
	//  NVAPI_GPU_BUS_TYPE_AGP     
	//  NVAPI_GPU_BUS_TYPE_PCI_EXPRESS     
	//  NVAPI_GPU_BUS_TYPE_FPCI    

	//  
	int GetBusType(int nIndex);

	//  
	//PCI identifiers associated with this GPU.  
	//  
	bool GetPCIIdentifiers(int nIndex, ULONG32&DeviceId, ULONG32 &SubSystemId, ULONG32 &RevisionId, ULONG32&ExtDeviceId);

	//  
	//the fan speed tachometer reading for the specified physical GPU  
	//  
	int GetTachReading(int nIndex);

	int GetGpuDynamicInfo(int index, NV_GPU_DYNAMIC_PSTATES_INFO_EX &PstatesInfo);
private:
	void Init_THERMAL_SETTINGS(NV_GPU_THERMAL_SETTINGS& ThermalSettings, NvU32 Version);



private:
	bool m_InitSucceed;
	NvPhysicalGpuHandle physicalGPUs[NVAPI_MAX_PHYSICAL_GPUS];
	NvU32 physicalGPUCount;
	NV_GPU_DYNAMIC_PSTATES_INFO_EX PstatesInfo;
};




