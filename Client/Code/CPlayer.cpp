#include "pch.h"
#include "CPlayer.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	//_vec3 vPos(64.f, 0.f, 64.f);
	//m_pTransformCom->Set_Pos(INFO_POS, &vPos);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vLook, vRight;
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);

	_vec3 vCamRight, vCamLook;
	vCamRight = CCameraMgr::GetInstance()->GetRightVector();
	vCamLook = CCameraMgr::GetInstance()->GetLookVector();
	D3DXVec3Normalize(&vCamRight, &vCamRight);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	if (CInputMgr::GetInstance()->KeyPress('A'))
	{
		m_pTransformCom->Move_Pos(&vCamRight, -10.f, fTimeDelta);
	}
	if (CInputMgr::GetInstance()->KeyPress('D'))
	{
		m_pTransformCom->Move_Pos(&vCamRight, 10.f, fTimeDelta);
	}
	if (CInputMgr::GetInstance()->KeyPress('W'))
	{
		m_pTransformCom->Move_Pos(&vCamLook, 10.f, fTimeDelta);
	}
	if (CInputMgr::GetInstance()->KeyPress('S'))
	{
		m_pTransformCom->Move_Pos(&vCamLook, -10.f, fTimeDelta);
	}

	_vec3 vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	//m_iFrameCounter++;
	//if (m_iFrameCounter >= m_iMaxFrame)
	//{
	//	printf("Real Pos: %.2f, %.2f, %.2f\n",
	//		vPos.x, vPos.y, vPos.z);
	//	m_iFrameCounter = 0;
	//}

	m_pTransformCom->Update_Component(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	//Key_Input(fTimeDelta);

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();


	//m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// RcTex
	pComponent = m_pBufferCom = static_cast<CRcTex*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// Transform
	pComponent = m_pTransformCom = static_cast<CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	// Texture
	pComponent = m_pTextureCom = static_cast<CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });


	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotation(ROT_X, 180.f * fTimeDelta);

	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotation(ROT_X, -180.f * fTimeDelta);

	if (GetAsyncKeyState('W'))
		m_pTransformCom->Rotation(ROT_Y, 180.f * fTimeDelta);

	if (GetAsyncKeyState('S'))
		m_pTransformCom->Rotation(ROT_Y, -180.f * fTimeDelta);

	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotation(ROT_Z, 180.f * fTimeDelta);

	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotation(ROT_Z, -180.f * fTimeDelta);

	_vec3	vUp;
	m_pTransformCom->Get_Info(INFO_UP, &vUp);

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), 10.f, fTimeDelta);

	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), -10.f, fTimeDelta);

}


CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pBackGround = new CPlayer(pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
