#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CRcTerrainTex : public CVIBuffer
{
protected:
	explicit CRcTerrainTex();
	explicit CRcTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTerrainTex(const CRcTerrainTex& rhs);
	virtual ~CRcTerrainTex();

public:
	virtual HRESULT		Ready_Buffer();
	virtual void		Render_Buffer();
	void Set_HeightMap(vector<int>& vecHeightMap);
public:
	static CRcTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent* Clone();

private:
	virtual void	Free();
};

END
