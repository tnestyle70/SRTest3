#include "pch.h"
#include "CJs.h"

CJs::CJs(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CJs::CJs(const CJs& rhs)
	: CGameObject(rhs)
{
}