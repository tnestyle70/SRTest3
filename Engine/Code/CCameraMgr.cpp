#include "CCameraMgr.h"
#include "CInputMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
	: m_pGraphicDev(nullptr)
	, m_pTargetTrans(nullptr)
	, m_fPitch(0.f)
	, m_fYaw(0.f)
	, m_fMouseSensitivity(0.002f)
	, m_vEye(0.f, 0.f, 0.f)
	, m_vAt(0.f, 0.f, 0.f)
	, m_vUp(0.f, 1.f, 0.f)
	, m_vLook(0.f, 0.f, 0.f)
	, m_vRight(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CCameraMgr::~CCameraMgr()
{
}

HRESULT CCameraMgr::Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGraphicDev = pGraphicDev;
	m_pGraphicDev->AddRef();

	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(90.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	//m_vEye = D3DXVECTOR3(64.f, 50.f, 64.f);

	return S_OK;
}

void CCameraMgr::Update(const _float fTimeDelta)
{
	if (CInputMgr::GetInstance()->KeyDown(VK_TAB))
	{
		m_bEditorMode = !m_bEditorMode;
	}
	//에디터 모드일 경우 update 하지 않도록 설정
	if (m_bEditorMode)
		return;

	//카메라 업데이트
	if (!m_pTargetTrans)
		return;
	UpdateMouseInput(fTimeDelta);
	UpdateFPSCamera(fTimeDelta);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCameraMgr::Set_Target(CTransform* pTrans)
{
	if (!pTrans)
		return;
	m_pTargetTrans = pTrans;
}

void CCameraMgr::UpdateMouseInput(const _float fTimeDelta)
{
	//마우스의 이동에 따른 x, y의 차이 구해서 설정해주기
	POINT ptMouse, ptCenter;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);
	
	ptCenter.x = WINCX >> 1;
	ptCenter.y = WINCY >> 1;
	//마우스 X, Y 축 기준으로 회전
	m_fYaw += (ptMouse.x - ptCenter.x) * m_fMouseSensitivity;
	m_fPitch -= (ptMouse.y - ptCenter.y) * m_fMouseSensitivity;
	//클라이언트 좌표 -> 스크린 좌표로 변환
	ClientToScreen(m_hWnd, &ptCenter);
	//마우스를 새로운 중앙점으로 이동
	SetCursorPos(ptCenter.x, ptCenter.y);
}

void CCameraMgr::UpdateFPSCamera(const _float fTimeDelta)
{
	//m_fYaw, m_fPitch의 값을 가지고 1인칭 시점의 카메라 업데이트	
	//player transform을 사용해서 eye, look / up, right, look을 계산해서 업데이트
	_vec3 vPlayerPos; 
	m_pTargetTrans->Get_Info(INFO_POS, &vPlayerPos);
	//플레이어 위치로 카메라 위치 설정
	m_vEye = vPlayerPos;
	//방향 벡터 계산 - Y축 기준 회전과 X축 기준 회전의 결과를 동시에 곱해서 방향 벡터를 구함
	//Yaw - Y축 기준 회전(x 변화), Pitch - X축 기준 회전(Y 변화)
	//고개를 들 수록 수평 반지름 cos(pitch)가 줄고, Y가 sin(pitch)로 생김
	//따라서 cosf(m_fPitch)를 곱해서 줄어드는 비율만큼 점은 이동 시킨다!
	//Yaw만 잇을 경우 - x = sin(yaw), z = cos(yaw), y = 0
	//pitch가 추가되면 고개를 들 수록 반지름이 줄어들음, 그 비율을 cosf(m_fPitch)로 곱해서 보정하는 것임
	m_vLook.x = cosf(m_fPitch) * sinf(m_fYaw);
	m_vLook.y = sinf(m_fPitch);
	m_vLook.z = cosf(m_fPitch) * cosf(m_fYaw);
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	//오른쪽 벡터 계산 - 플레이어를 이동하는데 사용, up벡터와 look 벡터의 외적을 통해서 구함
	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	//카메라가 바라보는 방향 구하기?
	m_vAt = m_vEye + m_vLook;
	//카메라가 바라보는 방향을 구해서 그 방향으로 정규화
	//view 행렬이 세상 전체를 카메라 시점에 맞게 반대로 변환!
	D3DXMatrixLookAtLH(&m_matView, &m_vEye, &m_vAt, &m_vUp);
}

_vec3 CCameraMgr::GetLookVector() const
{
	return _vec3(m_vLook.x, m_vLook.y, m_vLook.z);
}

_vec3 CCameraMgr::GetRightVector() const
{
	return _vec3(m_vRight.x, m_vRight.y, m_vRight.z);
}

void CCameraMgr::Free()
{
}
