#include "CInputMgr.h"

IMPLEMENT_SINGLETON(CInputMgr)

CInputMgr::CInputMgr()
{
	memset(m_bPrevKey, 0, sizeof(m_bPrevKey));
	memset(m_bCurrentKey, 0, sizeof(m_bCurrentKey));
}

CInputMgr::~CInputMgr()
{
}

void CInputMgr::Initialize()
{
}

void CInputMgr::Update()
{
	//이전 프레임 상태 백업
	memcpy(m_bPrevKey, m_bCurrentKey, sizeof(m_bPrevKey));
	//이번 프레임 상태 갱신
	for (int i = 0; i < 255; ++i)
	{
		int iVK = m_bPrevKey[i];
		SHORT sState = GetAsyncKeyState(iVK);
		//최상위 비트가 눌렸는지 여부 확인
		m_bCurrentKey[i] = (sState & 0x8000) != 0;
	}
}

bool CInputMgr::KeyPress(int vkCode) const
{
	SHORT eState = GetAsyncKeyState(vkCode);
	return(eState & 0x8000) != 0; //0x8000이면 1 true, 눌리지 않았으면 false 0이 true
}

bool CInputMgr::KeyDown(int vkCode) const
{
	//VK 전용 배열
	static std::unordered_map<int, bool> prevState;
	bool current = (GetAsyncKeyState(vkCode) & 0x8000) != 0;
	bool prev = prevState[vkCode];
	prevState[vkCode] = current;
	return current && !prev;
}

bool CInputMgr::KeyUp(int vkCode) const
{
	//VK 전용 배열
	static std::unordered_map<int, bool> prevState;
	bool current = (GetAsyncKeyState(vkCode) & 0x8000) != 0;
	bool prev = prevState[vkCode];
	prevState[vkCode] = current;
	return !current && prev;
}

void CInputMgr::Free()
{
}
