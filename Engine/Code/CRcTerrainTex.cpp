#include "CRcTerrainTex.h"

CRcTerrainTex::CRcTerrainTex()
{
}

CRcTerrainTex::CRcTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{
}

CRcTerrainTex::CRcTerrainTex(const CRcTerrainTex& rhs)
	: CVIBuffer(rhs)
{
}

CRcTerrainTex::~CRcTerrainTex()
{
}

HRESULT CRcTerrainTex::Ready_Buffer()
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 129 * 129;
	m_dwTriCnt = 128 * 128 * 2; //왜 이렇게 되는 거지?
	m_dwFVF = FVF_TEX;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertex = NULL;

	// &pVertex : 버텍스 버퍼에 저장된 버텍스 중 첫번째 버텍스의 주소를 얻어옴
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	//129 * 129 사이즈의 vertex buffer 생성
	for (int z = 0; z < 129; ++z)
	{
		for (int x = 0; x < 129; ++x)
		{
			int iIndex = z * 129 + x;
			//위치 정보
			pVertex[iIndex].vPosition.x = (float)x;
			pVertex[iIndex].vPosition.y = 0.f;
			pVertex[iIndex].vPosition.z = (float)z;
			//UV 정보 - x, z값을 128로 나눠서 지형 전체에 텍스쳐가 쫙 펴지게 설정(0 ~ 1)
			pVertex[iIndex].vTexUV = { (float)x / (128), (float)z / (128)};
		}
	}
	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int iIndex = 0; 
	for (int z = 0; z < 128; ++z)
	{
		for (int x = 0; x < 128; ++x)
		{
			//LT -> RT -> LB
			pIndex[iIndex]._0 = z * 129 + x;
			pIndex[iIndex]._1 = z * 129 + (x + 1);
			pIndex[iIndex]._2 = (z + 1) * 129 + x;
			iIndex++;
			//RT -> LB -> RB
			pIndex[iIndex]._0 = z * 129 + (x + 1);
			pIndex[iIndex]._1 = (z + 1) * 129 + x;
			pIndex[iIndex]._2 = (z + 1) * 129 + (x + 1);

			//삼각형 인덱스 증가
			iIndex++;
		}
	}

	m_pIB->Unlock();

	return S_OK;
}

void CRcTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void CRcTerrainTex::Set_HeightMap(vector<int>& vecHeightMap)
{
	VTXTEX* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (int z = 0; z < 129; ++z)
	{
		for (int x = 0; x < 129; ++x)
		{
			int iIndex = z * 129 + x;
			//bitmap은 아래에서 위로 저장되어있어서 뒤집어서 읽어야 함
			int iMapIndex = (128 - z) * 129 + x;
			pVertex[iIndex].vPosition.y = vecHeightMap[iMapIndex] / 255.f * 30.f;
		}
	}

	m_pVB->Unlock();
}

CRcTerrainTex* CRcTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTerrainTex* pRcTex = new CRcTerrainTex(pGraphicDev);

	if (FAILED(pRcTex->Ready_Buffer()))
	{
		Safe_Release(pRcTex);
		MSG_BOX("pRcTerrain Create Failed");
		return nullptr;
	}

	return pRcTex;
}

CComponent* CRcTerrainTex::Clone()
{
	return new CRcTerrainTex(*this);
}

void CRcTerrainTex::Free()
{
	CVIBuffer::Free();
}
