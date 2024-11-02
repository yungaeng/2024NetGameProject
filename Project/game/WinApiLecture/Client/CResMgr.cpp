#include "pch.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CSound.h"

CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	Safe_Delete_Map(m_mapTex);
	Safe_Delete_Map(m_mapSound);
	// 맵을 순회하면서 리소스들을 지워야 함
	/*map<wstring, CTexture*>::iterator iter = m_mapTex.begin();
	for (; iter != m_mapTex.end(); ++iter)
	{
		delete iter->second;
	}*/
}
CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
	// 찾았다면 그 자리의 이터레이터를 반환
	map<wstring,CRes*>::iterator iter = m_mapTex.find(_strKey);
	
	// 만약 찾지 못해서 m_mapTex의 end이터레이터가 반환되었다면,
	if (iter == m_mapTex.end())
	{
		return nullptr;
	}
	
	// 찾았다면 이터레이터의 두번쨰 요소 (CTexture*) 반환
	return (CTexture*)iter->second;
}

CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	// 텍스쳐가 이미 있는 경우
	CTexture* pTex = FindTexture(_strKey);
	if (nullptr != pTex)
	{
		return pTex;
	}
	// 없으면 경로 찾아서
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	// 새로운 텍스쳐 만든다음에 그 텍스쳐에 찾은 경로를 로드해서 받아옴
	pTex = new CTexture;
	pTex->Load(strFilePath);
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);

	m_mapTex.insert(make_pair(_strKey, pTex));
	return pTex;
}


CSound* CResMgr::LoadSound(const wstring& _strKey, const wstring& _strRelativePath)
{
	// 텍스쳐가 이미 있는 경우
	CSound* pSound = FindSound(_strKey);
	if (nullptr != pSound)
	{
		return pSound;
	}
	// 없으면 경로 찾아서
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	// 새로운 텍스쳐 만든다음에 그 텍스쳐에 찾은 경로를 로드해서 받아옴
	pSound = new CSound;
	pSound->Load(strFilePath);
	pSound->SetKey(_strKey);
	pSound->SetRelativePath(_strRelativePath);

	m_mapSound.insert(make_pair(_strKey, pSound));
	return pSound;
}

CSound* CResMgr::FindSound(const wstring& _strKey)
{
		// 찾았다면 그 자리의 이터레이터를 반환
	map<wstring,CRes*>::iterator iter = m_mapSound.find(_strKey);
	
	// 만약 찾지 못해서 m_mapTex의 end이터레이터가 반환되었다면,
	if (iter == m_mapSound.end())
	{
		return nullptr;
	}
	
	// 찾았다면 이터레이터의 두번쨰 요소 (CTexture*) 반환
	return (CSound*)iter->second;
}
