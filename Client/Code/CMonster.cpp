#include "pch.h"
#include "CMonster.h"
#include "CManagement.h"
#include "CProtoMgr.h"
#include "CInputMgr.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMonster::CMonster(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	return S_OK;
}

_int CMonster::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	Engine::CTransform* pPlayerTransformCom = dynamic_cast<CTransform*>
		(CManagement::GetInstance()->Get_Component(ID_DYNAMIC, L"GameLogic_Layer", L"Player", L"Com_Transform"));
		
	if (nullptr == pPlayerTransformCom)
		return;

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	_vec3	vMonsterPos;
	m_pTransformCom->Get_Info(INFO_POS, &vMonsterPos);

	_vec3	vDir = vPlayerPos - vMonsterPos;

	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), 5.f, fTimeDelta);

	//m_iFrameCounter++;
	//if (m_iFrameCounter >= m_iMaxFrame)
	//{
	//	printf("Real Pos: %.2f, %.2f, %.2f\n",
	//		vMonsterPos.x, vMonsterPos.y, vMonsterPos.z);
	//	m_iFrameCounter = 0;
	//}

	m_pTransformCom->Update_Component(fTimeDelta);

	//if(CInputMgr::GetInstance()->KeyDown('T'))
	//m_pTransformCom->Chase_Target(&vPlayerPos, 0.1f, fTimeDelta);
}

void CMonster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pBufferCom->Render_Buffer();
	
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CMonster::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// TriCol
	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TriCol"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* pMonster = new CMonster(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("pMonster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster::Free()
{
	CGameObject::Free();
}
