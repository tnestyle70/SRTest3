#include "pch.h"
#include "CLoading.h"
#include "CProtoMgr.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bFinish(false), m_eLoadingID(LOADING_STAGE)
{
	ZeroMemory(&m_szLoading, sizeof(m_szLoading));
	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	//임계 영역 자체를 사용할 수 있도록 초기화
	InitializeCriticalSection(&m_Critical);

	m_eLoadingID = eID;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0,
		Thread_Main, //구동할 쓰레드 함수, 함수를 인자로 전달
		this, //3번 매개변수를 통해서 가공할 데이터의 주소, void*로 받는 이유는
        //_beginthreadex가 범용 함수이기 때문에, 모든 타입의 데이터를 넘길 수 있도록 하고
        //받는 쪽에서 reinterpret_cast로 명시적 형변환을 해준다
		0, NULL);

	return S_OK;
}

_uint CLoading::Loading_Stage()
{
    lstrcpy(m_szLoading, L"Buffer Loading.....................................");

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev))))
        return E_FAIL;


    lstrcpy(m_szLoading, L"Texture Loading.....................................");

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_PlayerTexture", Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg"))))
        return E_FAIL;

    lstrcpy(m_szLoading, L"Etc Loading.....................................");

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;


    lstrcpy(m_szLoading, L"Loading Complete !!!!");

    m_bFinish = true;

    return 0;
}
//this를 넘겨서, 스레드 함수가 static 이어서 클래스의 멤버 변수에 접근하지 못하기 때문에, 
//특정 객체의 데이터에 접근할 수 있도록 하기 위함!
unsigned int CLoading::Thread_Main(void* pArg)
{
    //void*인 pArg를 명시적으로 형변환을 해서 사용
    CLoading* pLoading = reinterpret_cast<CLoading*>(pArg);

    int iFlag(0);
    //임계 영역에 진입
    EnterCriticalSection(pLoading->Get_Critical());
    //로딩 진행
    switch (pLoading->Get_LoadingID())
    {
    case LOADING_STAGE:
        iFlag = pLoading->Loading_Stage();
        break;
    }
    //임계 영역에서 벗어나기
    LeaveCriticalSection(pLoading->Get_Critical());

    return iFlag; //0을 리턴할 경우 endthreadex가 자동으로 호출
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
    CLoading* pLoading = new CLoading(pGraphicDev);

    if (FAILED(pLoading->Ready_Loading(eID)))
    {
        Safe_Release(pLoading);

        MSG_BOX("CLoading Create Failed");
        return nullptr;
    }

	return pLoading;
}

void CLoading::Free()
{
    //thread가 반환될 때까지 대기
    WaitForSingleObject(m_hThread, INFINITE);

    CloseHandle(m_hThread);

    DeleteCriticalSection(&m_Critical);

    Safe_Release(m_pGraphicDev);
}
