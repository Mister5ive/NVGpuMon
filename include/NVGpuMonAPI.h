// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� NVGPUMON_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// NVGPUMON_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef NVGPUMON_EXPORTS
#define NVGPUMON_API __declspec(dllexport)
#else
#define NVGPUMON_API __declspec(dllimport)
#endif

#pragma once  
#include"NVGpuMon.h"

//�ӿ�

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	NVGPUMON_API NVGpuMon* NVGpuMonCreate(void);
	NVGPUMON_API void NVGpuMonDestroy(NVGpuMon*);


#ifdef __cplusplus
}
#endif // __cplusplus
