#pragma once
#include "CGameObject.h"

class CJs : public CGameObject
{
private:
	explicit CJs(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CJs(const CJs& rhs);
	virtual ~CJs();
};
