#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CInputMgr : public CBase
{
	DECLARE_SINGLETON(CInputMgr)
private:
	explicit CInputMgr();
	virtual ~CInputMgr();
public:
	void Initialize();
	void Update();
public:
	_bool KeyPress(int iVKCode) const;
	_bool KeyDown(int iVKCode) const;
	_bool KeyUp(int iVKCode) const;
private:
	bool m_bPrevKey[256];
	bool m_bCurrentKey[256];
private:
	virtual void Free();
};

END
