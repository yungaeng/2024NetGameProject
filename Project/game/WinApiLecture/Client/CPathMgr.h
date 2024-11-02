#pragma once
class CPathMgr
{
	SINGLE(CPathMgr);

private:
	wchar_t m_szContentPath[256];	// 윈도우 최대 제한 255를 따라감
	wchar_t m_szRelativePath[256];
public:
	void init();
	// 훼손방지를 위한 const
	const wchar_t* GetContentPath() { return m_szContentPath; }

	// 절대경로 부분에서 상대경로 부분만 떼서 리턴해주는 함수
	wstring GetRelativePath(const wchar_t* _filepath);

};

