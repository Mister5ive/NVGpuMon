// NVGpuMon.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "NVGpuMon.h"
#include"NVGpuMonAPI.h"





NVGpuMon* NVGpuMonCreate()
{
	NVGpuMon * pInstance = new NVGpuMonImp();
	if (pInstance != NULL)
		return pInstance;
}

void NVGpuMonDestroy(NVGpuMon* pInstance)
{
	if (pInstance != NULL)
		delete	pInstance;
}




NVGpuMonImp::NVGpuMonImp(void) :m_InitSucceed(false)
{
	//初始化，nvapi.dll只能动态加载  
	m_InitSucceed = NvAPI_Initialize() == NVAPI_OK ? true : false;
}

NVGpuMonImp::~NVGpuMonImp(void)
{
	if (m_InitSucceed)
	{
		NvAPI_Unload();
	}
}


int NVGpuMonImp::GetGpuDynamicInfo(int index, NV_GPU_DYNAMIC_PSTATES_INFO_EX &PstatesInfo)
{
	PstatesInfo.version = NV_GPU_DYNAMIC_PSTATES_INFO_EX_VER;
	int ErrorCode = 0;
	ErrorCode = NvAPI_GPU_GetDynamicPstatesInfoEx(physicalGPUs[index], &PstatesInfo);
	return ErrorCode;
	//unsigned long p = PstatesInfo.utilization[index].percentage	
}

bool NVGpuMonImp::GetInterfaceVersionString(LPSTR strVer)
{
	assert(m_InitSucceed);
	return NvAPI_GetInterfaceVersionString(strVer) == NVAPI_OK ? true : false;
}
bool NVGpuMonImp::GetDisplayDriverVersion()
{
	//NvAPI_Status status;
	//NV_DISPLAY_DRIVER_VERSION version = { 0 };
	//version.version = NV_DISPLAY_DRIVER_VERSION_VER;
	////status = NvAPI_GetDisplayDriverVersion(NVAPI_DEFAULT_HANDLE, &version);
	////status = NvAPI_SYS_GetDriverAndBranchVersion(NVAPI_DEFAULT_HANDLE, &version);
	//if (status != NVAPI_OK)
	//{
	//	NvAPI_ShortString string;
	//	NvAPI_GetErrorMessage(status, string);
	//	return false;
	//}

	return true;
}



int NVGpuMonImp::GetPhyGpuCount()
{
	NvAPI_Status status;
	status = NvAPI_EnumPhysicalGPUs(physicalGPUs, &physicalGPUCount);
	if (status != NVAPI_OK)
	{
		return -1;
	}

	return physicalGPUCount;
}

void NVGpuMonImp::Init_THERMAL_SETTINGS(NV_GPU_THERMAL_SETTINGS& ThermalSettings, NvU32 Version)
{
	ZeroMemory(&ThermalSettings, sizeof(NV_GPU_THERMAL_SETTINGS));
	ThermalSettings.version = Version;
	ThermalSettings.count = NVAPI_MAX_THERMAL_SENSORS_PER_GPU;
	//  ThermalSettings.sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU].controller = NVAPI_THERMAL_CONTROLLER_UNKNOWN;  
	//  ThermalSettings.sensor[NVAPI_MAX_THERMAL_SENSORS_PER_GPU].target = NVAPI_THERMAL_TARGET_GPU;  


}
bool NVGpuMonImp::GetGpuTemperature(int nIndex, int& nMinTemp, int& nCurrentTemp, int& nMaxTemp)
{

	NvAPI_Status status;
	NvU32 temp = 0;
	NV_GPU_THERMAL_SETTINGS ThermalSettings = { 0 };

	Init_THERMAL_SETTINGS(ThermalSettings, NV_GPU_THERMAL_SETTINGS_VER);


	status = NvAPI_GPU_GetThermalSettings(physicalGPUs[nIndex],
		//nvGPUHandle,  
		//NVAPI_THERMAL_TARGET_ALL,  
		0,
		&ThermalSettings);
	if (NVAPI_INCOMPATIBLE_STRUCT_VERSION == status)
	{


		Init_THERMAL_SETTINGS(ThermalSettings, NV_GPU_THERMAL_SETTINGS_VER_1);

		status = NvAPI_GPU_GetThermalSettings(physicalGPUs[nIndex],
			//nvGPUHandle,  
			//NVAPI_THERMAL_TARGET_ALL,  
			0,
			&ThermalSettings);
		if (status != NVAPI_OK)
		{
			NvAPI_ShortString string;
			NvAPI_GetErrorMessage(status, string);
			//printf("NVAPI NvAPI_GetThermalSettings %s/n", string); //////////////////////////////// NVAPI_INCOMPATIBLE_STRUCT_VERSION   
			return false;
		}
		nCurrentTemp = ThermalSettings.sensor[0].currentTemp;
		nMinTemp = ThermalSettings.sensor[0].defaultMinTemp;
		nMaxTemp = ThermalSettings.sensor[0].defaultMaxTemp;

		return true;
	}
	else if (status == NVAPI_OK)
	{

		nCurrentTemp = ThermalSettings.sensor[0].currentTemp;
		nMinTemp = ThermalSettings.sensor[0].defaultMinTemp;
		nMaxTemp = ThermalSettings.sensor[0].defaultMaxTemp;

		return true;
	}

	return false;
}
//  
//Get physical full name by index  
//  
int NVGpuMonImp::GetGpuName(int nIndex, LPSTR szName, int nBuffSize)
{
	NvAPI_Status status;
	//  
	//获取显卡名称，"NVIDIA"  
	//  
	NvAPI_ShortString Name;
	status = NvAPI_GPU_GetFullName(physicalGPUs[nIndex], Name);
	if (status == NVAPI_OK)
	{
		if (nBuffSize < strlen(Name))
			return -1;

		strcpy(szName, Name);
		return 1;
	}


	return 0;
}


//  
//the GPU type (integrated or discrete)  
//  
int NVGpuMonImp::GetGpuType(int nIndex)
{
	NvAPI_Status status;
	NV_GPU_TYPE type;
	status = NvAPI_GPU_GetGPUType(physicalGPUs[nIndex], &type);
	if (status == NVAPI_OK)
	{
		return type;
	}

	return NV_SYSTEM_TYPE_GPU_UNKNOWN;
}
//  
//Get system type(notebook GPU or a desktop GPU)  
//  
int NVGpuMonImp::GetSysType(int nIndex)
{
	NvAPI_Status status;
	NV_SYSTEM_TYPE type;
	status = NvAPI_GPU_GetSystemType(physicalGPUs[nIndex], &type);
	if (status == NVAPI_OK)
	{
		return type;
	}

	return NV_SYSTEM_TYPE_UNKNOWN;
}

//  
//the virtual size of framebuffer in KB. This includes the physical RAM plus any system RAM that has been dedicated for use by the GPU.  
//  
int NVGpuMonImp::GetVirtualFrameBufferSize(int nIndex)
{
	NvAPI_Status status;
	NvU32 Size = -1;
	status = NvAPI_GPU_GetVirtualFrameBufferSize(physicalGPUs[nIndex], &Size);
	if (status == NVAPI_OK)
	{
		return Size;
	}

	return -1;

}

//  
//the physical size of framebuffer in KB. This does NOT include any system RAM that may be dedicated for use by the GPU  
//  
int NVGpuMonImp::GetPhysicalFrameBufferSize(int nIndex)
{
	NvAPI_Status status;
	NvU32 Size = -1;
	status = NvAPI_GPU_GetPhysicalFrameBufferSize(physicalGPUs[nIndex], &Size);
	if (status == NVAPI_OK)
	{
		return Size;
	}

	return -1;
}

//  
//the AGP aperture in megabytes.  
//  
int NVGpuMonImp::GetAGPAperture(int nIndex)
{
	NvAPI_Status status;
	NvU32 Size = -1;
	status = NvAPI_GPU_GetAGPAperture(physicalGPUs[nIndex], &Size);
	if (status == NVAPI_OK)
	{
		return Size;
	}

	return -1;

}

//  
//the current AGP Rate (0 = AGP not present, 1 = 1x, 2 = 2x, etc.)  
//  
int NVGpuMonImp::GetCurrentAGPRate(int nIndex)
{
	NvAPI_Status status;
	NvU32 Rate = -1;
	status = NvAPI_GPU_GetCurrentAGPRate(physicalGPUs[nIndex], &Rate);
	if (status == NVAPI_OK)
	{
		return Rate;
	}

	return -1;
}

//  
//Returns the ID of the bus associated with this GPU  
//  
int NVGpuMonImp::GetBusId(int nIndex)
{
	NvAPI_Status status;
	NvU32 BusId = -1;
	status = NvAPI_GPU_GetBusId(physicalGPUs[nIndex], &BusId);
	if (status == NVAPI_OK)
	{
		return BusId;
	}

	return -1;

}

//  
// Returns the ID of the bus slot associated with this GPU.  
//  
int NVGpuMonImp::GetBusSlotId(int nIndex)
{
	NvAPI_Status status;
	NvU32 BusSlotId = -1;
	status = NvAPI_GPU_GetBusSlotId(physicalGPUs[nIndex], &BusSlotId);
	if (status == NVAPI_OK)
	{
		return BusSlotId;
	}

	return -1;
}

//  
//returns the type of bus associated with this GPU.  
//  
int NVGpuMonImp::GetBusType(int nIndex)
{
	NvAPI_Status status;
	NV_GPU_BUS_TYPE type;
	status = NvAPI_GPU_GetBusType(physicalGPUs[nIndex], &type);
	if (status == NVAPI_OK)
	{
		return type;
	}

	return NVAPI_GPU_BUS_TYPE_UNDEFINED;

}

//  
//PCI identifiers associated with this GPU.  
//NvU32 *  pDeviceId,    
//NvU32 *  pSubSystemId,    
//NvU32 *  pRevisionId,    
//NvU32 *  pExtDeviceId   

//  
bool NVGpuMonImp::GetPCIIdentifiers(int nIndex, ULONG32&DeviceId, ULONG32 &SubSystemId, ULONG32 &RevisionId, ULONG32&ExtDeviceId)
{

	NvU32 Value = 0;
	NvAPI_Status status;
	status = NvAPI_GPU_GetPCIIdentifiers(physicalGPUs[nIndex], (NvU32 *)&DeviceId, (NvU32 *)&SubSystemId, (NvU32 *)&RevisionId, (NvU32 *)&ExtDeviceId);
	if (status == NVAPI_OK)
	{
		return true;
	}

	return false;
}

//  
//the fan speed tachometer reading for the specified physical GPU  
//  
int NVGpuMonImp::GetTachReading(int nIndex)
{
	NvU32 Value = 0;
	NvAPI_Status status;
	status = NvAPI_GPU_GetTachReading(physicalGPUs[nIndex], &Value);
	if (status == NVAPI_OK)
	{
		return Value;
	}

	return -1;
}
