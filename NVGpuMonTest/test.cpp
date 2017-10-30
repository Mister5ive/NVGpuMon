#include"include/NVGpuMonAPI.h"

typedef NVGpuMon*			(*PNVGPUMONCREATE)();
typedef void				(*PNVGPUMONDESTROY)(NVGpuMon *);
typedef struct _NVGPUMON
{
	PNVGPUMONCREATE			m_NVGpuMonCreate;
	PNVGPUMONDESTROY		m_NVGpuMonDestroy;
}NVGPUMON;

NVGPUMON *m_pNVGpuMon = NULL;
NVGpuMon *m_pNVGpuMonIns = NULL;//ÊµÀý
void *_pNVApiDll = NULL;
 
#define _WIN64 1

bool load_nvgpumon_lib()
{
#if _WIN64
#ifdef UNICODE
	static LPCWSTR __NVApiDllName = L"../dll/x64/NVGpuMon.dll";
#else
	static LPCSTR __NVApiDllName = "../dll/x64/NVGpuMon.dll";
#endif//UNICODE
#else _WIN32
#ifdef UNICODE
	static LPCWSTR __NVApiDllName = L"../dll/x86/NVGpuMon.dll";
#else
	static LPCSTR __NVApiDllName = "../dll/x86/NVGpuMon.dll";
#endif//UNICODE
#endif
	_pNVApiDll = LoadLibrary(__NVApiDllName);
	if (_pNVApiDll == NULL)
	{
		printf("%s : %s LoadLibrary failed, please check file, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		printf("error code: %d\n", GetLastError());
		return false;
	}
	else
	{
		if (m_pNVGpuMon == NULL)
		{
			try{
				m_pNVGpuMon = new NVGPUMON;
			}
			catch (const std::bad_alloc& e)
			{
				printf("%s : %s malloc error:%s, line:%d!\n", __FILE__, __FUNCTION__, e.what(), __LINE__);
			}
		}
		if (m_pNVGpuMon == NULL)
		{
			printf("%s : %s malloc error, line:%d!\n", __FILE__, __FUNCTION__, __LINE__);
			return false;
		}
		memset(m_pNVGpuMon, 0, sizeof(m_pNVGpuMon));
		m_pNVGpuMon->m_NVGpuMonCreate = (PNVGPUMONCREATE)GetProcAddress((HMODULE)_pNVApiDll, "NVGpuMonCreate");
		m_pNVGpuMon->m_NVGpuMonDestroy = (PNVGPUMONDESTROY)GetProcAddress((HMODULE)_pNVApiDll, "NVGpuMonDestroy");
		if (m_pNVGpuMon->m_NVGpuMonCreate == NULL || m_pNVGpuMon->m_NVGpuMonDestroy == NULL)
		{
			printf("%s : %s NVGpuMon GetProcAddress failed, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		}

	}
	return true;
}

bool Init()
{
	if (!load_nvgpumon_lib())
		return false;
	if (m_pNVGpuMon->m_NVGpuMonCreate == NULL)
	{
		printf("%s : %s invalid param, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	m_pNVGpuMonIns = m_pNVGpuMon->m_NVGpuMonCreate();
	if (m_pNVGpuMonIns == NULL)
	{
		printf("%s : %s NVGpuMon create failed, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
}
bool Destroy()
{
	if (m_pNVGpuMon->m_NVGpuMonDestroy == NULL || m_pNVGpuMonIns == NULL)
	{
		printf("%s : %s invalid param, line: %d\n", __FILE__, __FUNCTION__, __LINE__);
		return false;
	}
	m_pNVGpuMon->m_NVGpuMonDestroy(m_pNVGpuMonIns);
	m_pNVGpuMonIns = NULL;
	if (m_pNVGpuMon != NULL)
		delete m_pNVGpuMon;
	if (_pNVApiDll)
	{
		FreeLibrary((HMODULE)_pNVApiDll);
		_pNVApiDll = NULL;
	}
	return true;
}

int main()
{
	if (!Init())
	{
		printf("pinit failed \n");
		getchar();

		return -1;
	}
	int phyCount = m_pNVGpuMonIns->GetPhyGpuCount();
	printf("nvidia gpu number: %d\n",phyCount);

	NV_GPU_DYNAMIC_PSTATES_INFO_EX PstatesInfo;
	if (phyCount > 0)
		while (1)
		{
			if(m_pNVGpuMonIns->GetGpuDynamicInfo(0, PstatesInfo) == 0)
				printf("percentage:%d%c\n", PstatesInfo.utilization[0].percentage, '%');
			else
				printf("get percentage error \n");
			Sleep(1000);
		}
	getchar();
	return 0;
}