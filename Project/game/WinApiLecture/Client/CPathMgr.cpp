#include "pch.h"
#include "CPathMgr.h"
#include "CCore.h"
CPathMgr::CPathMgr()
	:m_szContentPath{}
{
	init();
}
CPathMgr::~CPathMgr()
{
}

void CPathMgr::init()
{
	// 윈도우는 실행된 위치를 현재 디렉터리로 저장해 놓음, 그걸 받아오면 됨
	GetCurrentDirectory(255, m_szContentPath);
	// 상위 폴더로 나간 후,
	// +bin\\content\\
	// 문자열 다룰 겸 상위 폴더 반환 함수를 만들어보자. (라이브러리가 존재하지만)
	int iLen = (int)wcslen(m_szContentPath);
	// 크기니까 인덱스로 쓸 경우에는 하나 빼줘야 한다.
	for (int i = iLen - 1; i >= 0; --i)
	{
		if ('\\' == m_szContentPath[i])
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}
	// 널문자인 곳부터 문자열을 붙여준다
	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");

	/* 비쥬얼 스튜디오를 통해 디버깅하여 실행할 경우 경로가 다른 곳으로 감 (작업 디렉터리)
	 -> 속성 값에서 설정 (디버깅) - (작업 디렉터리) */
		SetWindowText(CCore::GetInst()->GetMainHwnd(), m_szContentPath);

}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;

	
	size_t iAbsLen = wcslen(m_szContentPath);	// 절대경로의 길이
	size_t iFullLen = strFilePath.length();

	// 절대경로부터, 완전한 파일 경로에서 절대경로의 길이만큼을 뺀 문자열을 얻어온다,
	// 즉 절대경로 이후에 이어지는 상대경로 만큼만을 가져온다.
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);

	return strRelativePath;
}
