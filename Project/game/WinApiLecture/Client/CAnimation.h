#pragma once
#include "global.h"


class CAnimator;
class CTexture;

// 프레임 당 정보 기록
struct tAnimFrm
{
	Vec2	vLT;			// 1. 좌상단
	Vec2	vSlice;			// 2. 자를 사이즈 영역
	Vec2	vOffset;		// 3. 프레임 별 offset
	float	fDuration;		// 4. 프레임 지속시간
};

class CAnimation
{
private:
	wstring				m_strName;	// 애니메이션 이름
	CAnimator*			m_pAnimator;// 자신을 가리키는 애니메이터
	CTexture*			m_pTex;		// Animation이 사용할 텍스쳐
	vector<tAnimFrm>	m_vecFrm;	// 모든 프레임 정보 저장
	int					m_iCurFrm;	// 현재 프레임
	float				m_fAccTime;	// 누적 시간
	bool				m_bFinish;	// 한 프레임 사이클을 다 돌았는지 여부
private:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	
public:
	const wstring& GetName() { return m_strName; }
	UINT GetMaxFream() { return (UINT)m_vecFrm.size(); }
	bool IsFinish() { return m_bFinish; }
	void SetFrame(int _iFrameIdx)
	{
		m_bFinish = false;
		m_iCurFrm = _iFrameIdx;
		m_fAccTime = 0.f;
	}
	// 현재 진행중인 애니메이션 프레임의 프레임 당 정보를 가져온다.
	// 레퍼런스로 반환해 원본 수정가능하게 하기
	tAnimFrm& GetFrame(int _iIdx) { return m_vecFrm[_iIdx]; }
public:
	void Save(const wstring& _strRelativPath);
	void Load(const wstring& _strRelativPath);
public:
	void update();
	void render(HDC _dc);
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep,float _fDuration, UINT _iFrameCount);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

