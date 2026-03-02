#include "pch.h"
#include "CMainApp.h"
#include "CLogo.h"
#include "CStage.h"
#include "CProtoMgr.h"
#include "CCameraMgr.h"

CMainApp::CMainApp()
    : m_pDeviceClass(nullptr), m_pGraphicDev(nullptr)
    , m_pManagementClass(CManagement::GetInstance())
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
    if (FAILED(Ready_DefaultSetting(&m_pGraphicDev)))
        return E_FAIL;

    if (FAILED(Ready_Scene(m_pGraphicDev)))
        return E_FAIL;

    return S_OK;
}

int CMainApp::Update_MainApp(const float& fTimeDelta)
{
    m_pManagementClass->Update_Scene(fTimeDelta);

    return 0;
}

void CMainApp::LateUpdate_MainApp(const float& fTimeDelta)
{
    m_pManagementClass->LateUpdate_Scene(fTimeDelta);
}

void CMainApp::Render_MainApp()
{
    m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

    m_pManagementClass->Render_Scene(m_pGraphicDev);

    m_pDeviceClass->Render_End();
}

HRESULT CMainApp::Ready_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev)
{
    if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
        return E_FAIL;

    m_pDeviceClass->AddRef();

    (*ppGraphicDev) = m_pDeviceClass->Get_GraphicDev();
    (*ppGraphicDev)->AddRef();

    (*ppGraphicDev)->SetRenderState(D3DRS_LIGHTING, FALSE);


    (*ppGraphicDev)->SetRenderState(D3DRS_ZENABLE, TRUE);       // Z 버퍼에 무조건 기록, Z버퍼 자동 정렬을 할 지 결정
    (*ppGraphicDev)->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);  // Z 버퍼에 Z값을 보관할 것인지 묻는 옵션

    CCameraMgr::GetInstance()->Ready_Camera(m_pGraphicDev, g_hWnd);

    return S_OK;
}

HRESULT CMainApp::Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    Engine::CScene* pLogo = CStage::Create(pGraphicDev);
    
    if (nullptr == pLogo)
        return E_FAIL;
    
    if (FAILED(m_pManagementClass->Set_Scene(pLogo)))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return E_FAIL;
    }

    return S_OK;
}

CMainApp* CMainApp::Create()
{

    CMainApp* pInstance = new CMainApp;

    if (FAILED(pInstance->Ready_MainApp()))
    {
        Safe_Release(pInstance);
        MSG_BOX("MainApp Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CMainApp::Free()
{
    Safe_Release(m_pDeviceClass);
    Safe_Release(m_pGraphicDev);

    CProtoMgr::GetInstance()->DestroyInstance();
    CFrameMgr::GetInstance()->DestroyInstance();
    CTimerMgr::GetInstance()->DestroyInstance();
    CManagement::GetInstance()->DestroyInstance();
    CGraphicDev::GetInstance()->DestroyInstance();
}
