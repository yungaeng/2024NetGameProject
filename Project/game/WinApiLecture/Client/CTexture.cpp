#include "pch.h"
#include "CTexture.h"
#include "CCore.h"

CTexture::CTexture()
	:m_hBit(0)
	, m_dc(0)
	, m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void CTexture::Load(const wstring& _strFilePath)
{
	// 해당 경로에 존재하는 이미지 파일을 메모리상에 올리고 비트맵으로 ID(H)를 받아와 가리킨다.
	m_hBit = (HBITMAP)LoadImage(nullptr,_strFilePath.c_str(),IMAGE_BITMAP,0,0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	// 로딩이 제대로 되지 않은 경우
	assert(m_hBit);

	// 비트맵과 연결할 DC	, 해당 비트맵과 연결할 DC 할당 받기
	m_dc = CreateCompatibleDC(CCore::GetInst()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);

	// 가로 세로 길이
	// 비트맵 : 비트맵 정보를 저장하는 구조체(핸들아님)
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);


	
}

