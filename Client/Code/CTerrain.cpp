#include "pch.h"
#include "CTerrain.h"
#include <fstream>
#include <vector>
#include <string>

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTerrain::~CTerrain()
{
}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(ReadBMPFile("../Bin/Resource/Texture/Terrain/Height1.bmp")))
	{
		MSG_BOX("Terrain Create Failed");
		return E_FAIL;
	}

	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pTransCom->Update_Component(fTimeDelta);

	return iExit;
}

void CTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_World());

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture(0);

	m_pRcTerrain->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTerrain::ReadBMPFile(string fileName)
{
	ifstream inFile(fileName.c_str(), ios_base::binary);
	if (!inFile) return E_FAIL;

	BITMAPFILEHEADER tBFH;
	BITMAPINFOHEADER tBIH;
	inFile.read((char*)&tBFH, sizeof(tBFH));
	inFile.read((char*)&tBIH, sizeof(tBIH));

	printf("BitCount: %d, Width: %d, Height: %d\n",
		tBIH.biBitCount, tBIH.biWidth, tBIH.biHeight);

	int iSize = m_iNumVertices * m_iNumVertices;

	// 32bit = 픽셀당 4바이트
	vector<BYTE> in(iSize * 4);
	inFile.read((char*)&in[0], iSize * 4);
	inFile.close();

	// R값만 추출해서 높이로 사용
	m_vecHeightMap.resize(iSize);
	for (int i = 0; i < iSize; ++i)
	{
		// BGRA 순서에서 R값 = [2]번째
		m_vecHeightMap[i] = in[i * 4 + 2];
	}

	return S_OK;
}

HRESULT CTerrain::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	//Transform
	pComponent = m_pTransCom = static_cast<CTransform*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//RcTerrainTexture
	pComponent = m_pRcTerrain = static_cast<CRcTerrainTex*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTerrain"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_RcTerrain", pComponent });
	//height 정보 넘겨주기
	m_pRcTerrain->Set_HeightMap(m_vecHeightMap);

	//CTexture
	pComponent = m_pTextureCom = static_cast<CTexture*>(
		CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_TerrainTexture"));
	if (!pComponent)
		return E_FAIL;
	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pTerrain = new CTerrain(pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}
	
	return pTerrain;
}

void CTerrain::Free()
{
}
