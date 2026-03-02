#pragma once
#include "CGameObject.h"
#include "CProtoMgr.h"

class CTerrain : public CGameObject
{
public:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain();
public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual void LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void Render_GameObject();
public:
	HRESULT ReadBMPFile(string fileName);
private:
	HRESULT Add_Component();
private:
	vector<int> m_vecHeightMap;
	int m_iNumVertices = 129; //129 * 129 사이즈의 크기
	Engine::CTransform* m_pTransCom;
	Engine::CRcTerrainTex* m_pRcTerrain;
	Engine::CTexture* m_pTextureCom;
public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};