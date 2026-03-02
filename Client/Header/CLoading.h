#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID {LOADING_STAGE, LOADING_END };
public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();
public:
	const _tchar* Get_String() { return m_szLoading; }
	LOADINGID Get_LoadingID() { return m_eLoadingID; }
	_bool Get_Finishe() { return m_bFinish; }
	CRITICAL_SECTION* Get_Critical() { return &m_Critical; }
public:
	HRESULT Ready_Loading(LOADINGID eID);
	_uint Loading_Stage();
public://매개변수로 함수를 호출, CALLEE, CALLER 존재, 함수 포인터를 넘겨서 호출
	static unsigned int CALLBACK Thread_Main(void* pArg);
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	_tchar m_szLoading[128]; //로딩 상태 문자열

	HANDLE m_hThread; //생성할 쓰레드 핸들
	LOADINGID m_eLoadingID;

	CRITICAL_SECTION m_Critical; //임계 영역(쓰레드가 동시에 접근할 가능성이 있는 영역)
	_bool m_bFinish; //로딩이 끝났는지를 알려주는 플래그
public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);
private:
	virtual void Free();
};