#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"

CAnimator::CAnimator()
	:m_pCurAnim(nullptr)
	, m_pOwner(nullptr)
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnim);
}




CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring,CAnimation*>::iterator iter = m_mapAnim.find(_strName);

	// 찾으려는 animation이 없는 경우
	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator::Play(const wstring& _strName, bool _Repeat)
{
	// 현재 애니메이션을 선택 , 선택했으니까 널포인터가 아니게 됨 -> update와 render 진행
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _Repeat;
}


void CAnimator::update()
{
	
}

void CAnimator::finalupdate()
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->update();
		// C++ 코드의 최적화 > && if문의 첫 조건이 false이면 뒷 조건은 체크하지 않는다. ★★★
		// 마찬가지로  || if문의 첫 조건이 true이면 뒷 조건은 상관없으므로 체크 x
		if (m_bRepeat && m_pCurAnim->IsFinish())
		{
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::render(HDC _dc)
{
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->render(_dc);
	}
}

void CAnimator::CreateAnimation(const wstring& _strName,CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep,float fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	// 이미 존재하는 경우
	assert(nullptr == pAnim);


	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep,fDuration, _iFrameCount);


	m_mapAnim.insert(make_pair(_strName, pAnim));
}

void CAnimator::LoadAnimation(const wstring& _strRelativePath)
{
	CAnimation* pAnim = new CAnimation;
	pAnim->Load(_strRelativePath);
	pAnim->m_pAnimator = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}




