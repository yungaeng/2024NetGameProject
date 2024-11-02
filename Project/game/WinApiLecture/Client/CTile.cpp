#include "pch.h"
#include "CTile.h"
#include "CTexture.h"


CTile::CTile()
	:m_pTileTex(nullptr)
	,m_iImgIdx(0)
{
	// 사이즈를 타일 사이즈로 기본 지정
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
}

CTile::~CTile()
{
}

void CTile::update()
{
}

void CTile::render(HDC _dc)
{
	// 보유한 텍스쳐가 없는 경우 띄우지 않음
	if (nullptr == m_pTileTex)
		return;
	UINT iWidth = m_pTileTex->Width();	// 텍스처의 가로 구해오기
	UINT iHeight = m_pTileTex->Height();// 텍스처의 세로	구해오기

	UINT iMaxCol = iWidth / TILE_SIZE;	// 구해온 가로를 타일 사이즈로 나누어 열의 개수 구하기
	UINT iMaxRow = iHeight / TILE_SIZE;	// 구해온 세로를 타일 사이즈로 나누어 행의 개수 구하기
	
	UINT iCurRow = m_iImgIdx / iMaxCol;	// 인덱스 / 열의 개수 = 몇번째 행(세로로 몇)인지 나타내는 인덱스
	UINT iCurCol = m_iImgIdx % iMaxCol;	// 인덱스 % 열의 개수 = 몇번째 열(가로로 몇)인지 나타내는 인덱스

	// 이미지 범위(최대 행의 개수)를 넘어선 인덱스
	if (iMaxRow <= iCurRow)
		assert(nullptr);

	Vec2 vRenderPos = CCamera::GetInst()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	BitBlt(
		_dc
		, (int)vRenderPos.x
		, (int)vRenderPos.y,
		(int)vScale.x
		, (int)vScale.y
		, m_pTileTex->GetDC()
		, iCurCol * TILE_SIZE
		, iCurRow * TILE_SIZE
		, SRCCOPY);
	
}

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);

}
