#pragma once
#include "CTransform.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)
private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();
public:
	HRESULT Ready_Camera(LPDIRECT3DDEVICE9 pGraphicDev, HWND hWnd);
	void Update(const _float fTimeDelta);
	//플레이어가 따라다닐 target의 설정
	void Set_Target(CTransform* pTrans);
	const _matrix& GetViewMatrix() { return m_matView; }
	const _matrix& GetProjMatrix() { return m_matProj; }
	_vec3 GetLookVector() const;
	_vec3 GetRightVector() const;
private:
	void UpdateMouseInput(const _float fTimeDelta);
	void UpdateFPSCamera(const _float fTimeDelta);
private:
	HWND m_hWnd; //graphicDev를 통해서 window 설정
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	CTransform* m_pTargetTrans;
	//카메라 회전
	_float m_fPitch;
	_float m_fYaw;
	_float m_fMouseSensitivity;
	//카메라 행렬
	_vec3 m_vEye; //카메라 위치
	_vec3 m_vAt; //플레이어 위치
	_vec3 m_vLook; // Z축
	_vec3 m_vRight; // X축
	_vec3 m_vUp; // Y축
	_matrix m_matView; //뷰 행렬
	_matrix m_matProj; //투영 행렬
	//에디터 모드
	_bool m_bEditorMode = false;
private:
	virtual void Free();
};

END