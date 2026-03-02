#include "pch.h"
#include "CStage.h"
#include "CBackGround.h"
#include "CProtoMgr.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CTerrain.h"
#include "CCameraMgr.h"

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CScene(pGraphicDev)
{
}

CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene()
{
    if (FAILED(Ready_Prototype()))
        return E_FAIL;

    if (FAILED(Ready_Environment_Layer(L"Environment_Layer")))
        return E_FAIL;

    if (FAILED(Ready_GameLogic_Layer(L"GameLogic_Layer")))
        return E_FAIL;

    if (FAILED(Ready_UI_Layer(L"UI_Layer")))
        return E_FAIL;

    //_matrix     matView, matProj;

    //_vec3   vEye{ 0.f, 0.f, -10.f };
    //_vec3   vAt{ 0.f, 0.f, 1.f };
    //_vec3   vUp{ 0.f, 1.f, 0.f };

    //D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    //m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

    //D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
    //m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

    return S_OK;
}

_int CStage::Update_Scene(const _float& fTimeDelta)
{
    _int iExit = Engine::CScene::Update_Scene(fTimeDelta);

    CCameraMgr::GetInstance()->Update(fTimeDelta);

    return iExit;
}

void CStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    Engine::CScene::LateUpdate_Scene(fTimeDelta);
}

void CStage::Render_Scene()
{
    Engine::CScene::Render_Scene();
}

HRESULT CStage::Ready_Environment_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    // 오브젝트 추가
    CGameObject* pGameObject = nullptr;

    //Terrain
    pGameObject = CTerrain::Create(m_pGraphicDev);
    if (!pGameObject)
        return E_FAIL; //지형 정보 Layer 쪽에 추가
    if (FAILED(pLayer->Add_GameObject(L"Terrain", pGameObject)))
        return E_FAIL;
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    // 오브젝트 추가
    CGameObject* pGameObject = nullptr;
    // player
    pGameObject = CPlayer::Create(m_pGraphicDev);
    if (nullptr == pGameObject)
        return E_FAIL;
    if (FAILED(pLayer->Add_GameObject(L"Player", pGameObject)))
        return E_FAIL;
    CCameraMgr::GetInstance()->Set_Target(dynamic_cast<CPlayer*>(pGameObject)->GetTransform());

    // monster
    pGameObject = CMonster::Create(m_pGraphicDev);

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(pLayer->Add_GameObject(L"Monster", pGameObject)))
        return E_FAIL;
   
    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar* pLayerTag)
{
    CLayer* pLayer = CLayer::Create();

    if (nullptr == pLayer)
        return E_FAIL;

    // 오브젝트 추가

    CGameObject* pGameObject = nullptr;

   

    m_mapLayer.insert({ pLayerTag, pLayer });

    return S_OK;
}

HRESULT CStage::Ready_Prototype()
{
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTex", Engine::CRcTex::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev))))
        return E_FAIL;

    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_PlayerTexture", 
        Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Player/Ma.jpg"))))
        return E_FAIL;
    //TerrainTexture component 추가
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_RcTerrain",
        Engine::CRcTerrainTex::Create(m_pGraphicDev))))
        return E_FAIL;
    if (FAILED(CProtoMgr::GetInstance()->Ready_Prototype(L"Proto_TerrainTexture",
        Engine::CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Bin/Resource/Texture/Terrain/Terrain0.png"))))
        return E_FAIL;

    return S_OK;
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CStage* pLogo = new CStage(pGraphicDev);

    if (FAILED(pLogo->Ready_Scene()))
    {
        Safe_Release(pLogo);
        MSG_BOX("Logo Create Failed");
        return nullptr;
    }

    return pLogo;
}

void CStage::Free()
{
    CScene::Free();
}
