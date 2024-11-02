#pragma once
#include "global.h"



class CRes
{
private:
	wstring m_strKey;			// 리소스 키
	wstring m_strRelativePath;	// 리소스 상대경로 (프로그램 폴더 내에서의 리소스 위치)

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }

public:
	CRes();

	// CRes가 삭제될 때 자식 쪽 소멸자에서 삭제되도록 가상함수로 작성
	virtual~CRes();
};

