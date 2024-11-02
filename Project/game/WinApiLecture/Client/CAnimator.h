#pragma once
class Cobject;
class CAnimation;
class CTexture;

class CAnimator
{

private:
	map<wstring, CAnimation*>	m_mapAnim;	// 모든 Animation
	CObject*					m_pOwner;	// Animator 소유 오브젝트의 주소
	CAnimation*					m_pCurAnim;	// 현재 재생중인 Animation
	bool						m_bRepeat;	// 반복재생 여부

public:
	void SetName(const wstring& _strName){}
	
	CObject* GetObj() { return m_pOwner; }

	void CreateAnimation(const wstring& _strName,CTexture* _pTex, Vec2 _vLT , Vec2 _vSliceSize,Vec2 _vStep, float fDuration,UINT _iFrameCount);
	void LoadAnimation(const wstring& _strRelativePath);
	CAnimation* FindAnimation(const wstring& _strName);
	void Play(const wstring& _strName,bool _Repeat);

	void update();
	void finalupdate();
	void render(HDC _dc);

public:
	CAnimator();
	~CAnimator();

	friend class CObject;
};

