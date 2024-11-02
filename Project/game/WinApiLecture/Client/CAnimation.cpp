#include "pch.h"

#include "CTimeMgr.h"

#include "CAnimation.h"
#include "CAnimator.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"
#include "CPathMgr.h"
#include "CResMgr.h"

CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	, m_pTex(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation::~CAnimation()
{
}



void CAnimation::Save(const wstring& _strRelativPath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativPath;
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(),L"wb");
	assert(pFile);

	// Animation의 이름을 저장한다. (데이터 직렬화)
	fprintf(pFile, "[Animation Name]\n");
	string strName = string(m_strName.begin(), m_strName.end());
	fprintf(pFile, strName.c_str());
	//SaveWString(m_strName, pFile);
	fprintf(pFile, "\n");

	// Animation이 사용할 텍스쳐의 키를 저장, 재부팅 시를 대비한 상대경로 저장
	fprintf(pFile, "[Texture Key]\n");
	strName = string(m_pTex->GetKey().begin(), m_pTex->GetKey().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");

	fprintf(pFile, "[Texture Path]\n");
	strName = string(m_pTex->GetRelativePath().begin(), m_pTex->GetRelativePath().end());
	fprintf(pFile, strName.c_str());
	fprintf(pFile, "\n");
	//SaveWString(m_pTex->GetKey(),pFile);
	//SaveWString(m_pTex->GetRelativePath(), pFile);


	// 프레임의 개수 저장
	fprintf(pFile, "[Frame Count]\n");
	fprintf(pFile, "%d\n",(int)m_vecFrm.size());
	//size_t iFrameCount = m_vecFrm.size();
	//fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// 각각의 프레임 정보들 저장
	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fprintf(pFile, "[Frame Index]\n");
		fprintf(pFile, "%d\n", (int)i);

		fprintf(pFile, "[Left Top]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vLT.x, (int)m_vecFrm[i].vLT.y);

		fprintf(pFile, "[Slice Size]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vSlice.x, (int)m_vecFrm[i].vSlice.y);

		fprintf(pFile, "[Offset]\n");
		fprintf(pFile, "%d %d\n", (int)m_vecFrm[i].vOffset.x, (int)m_vecFrm[i].vOffset.y);

		fprintf(pFile, "[Duration]\n");
		fprintf(pFile, "%f\n", m_vecFrm[i].fDuration);

		fprintf(pFile, "\n\n");

	}

	

	
	// 모든 프레임 정보 저장
	// 벡터의 시작주소로부터 벡터 데이터크기만큼을 벡터 전체 사이즈까지 파일에 저장
	/*fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);*/


	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativPath)
{

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativPath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	// 간단 함수 버전 - 파일에서 수정불가
	//// 문자열 읽어오는 함수 ,읽어와서 애니메이션의 이름으로 저장
	////LoadWString(m_strName, pFile);
	//

	//
	//wstring strTexKey,strTexPath; // 텍스처의 키(문자열) 받아오기
	//LoadWString(strTexKey, pFile);// 텍스처의 상대 경로 받아오기
	//LoadWString(strTexPath, pFile);
	//// 텍스처를 저장해놓고 프로그램 재부팅을 하면 ResMgr에 들어있던 텍스처 정보가 사라짐
	//// 따라서 키 뿐만 아니라 경로까지 받아놓고 Find가 아닌 Load를 통해 완벽히 받아온다.
	//m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);// 등록된 텍스처가 없으면 등록함
	//
	//// 프레임 개수
	//size_t iFrameCount = 0;
	//fread(&iFrameCount, sizeof(size_t), 1, pFile);
	//
	//// 모든 프레임 정보		- 한번에 넣으면 배열 확장에 쓰이는 자원을 낭비하지 않을 수 있음
	//m_vecFrm.resize(iFrameCount);		// 프레임 개수만큼 벡터 확장
	//fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);


	// 파일 수정 가능 버전
	char szBuff[256] = {};
	string str;
	// 문자열 한 줄씩 읽어오는 함수

	// 1. 애니메이션 이름 읽어오기
	FScanf(szBuff, pFile);	// 항목이 읽혀짐
	FScanf(szBuff, pFile);
	str = szBuff;
	m_strName = wstring(str.begin(), str.end());

	// 2. 텍스처 키
	FScanf(szBuff, pFile);	// 항목이 읽혀짐
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexKey = wstring(str.begin(), str.end());

	// 3. 텍스처 상대 경로
	FScanf(szBuff, pFile);	// 항목이 읽혀짐
	FScanf(szBuff, pFile);
	str = szBuff;
	wstring strTexPath = wstring(str.begin(), str.end());

	m_pTex = CResMgr::GetInst()->LoadTexture(strTexKey, strTexPath);


	// 4. 프레임 개수
	FScanf(szBuff, pFile);	// 항목이 읽혀짐
	int iFrameCount = 0;
	fscanf_s(pFile, "%d", &iFrameCount);
	

	// 각각의 프레임 정보들 불러오기
	tAnimFrm frm = {};
	for (int i = 0; i < iFrameCount; ++i)
	{
		POINT pt = {};
		while (true)
		{
			FScanf(szBuff, pFile);	// 계속 읽어옴
			// 두 문자열이 같으면 0
			if (!strcmp("[Frame Index]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
			}
			else if (!strcmp("[Left Top]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vLT = pt;
			}
			else if (!strcmp("[Slice Size]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vSlice = pt;
			}
			else if (!strcmp("[Offset]", szBuff))
			{
				fscanf_s(pFile, "%d", &pt.x);
				fscanf_s(pFile, "%d", &pt.y);
				frm.vOffset = pt;
			}
			else if (!strcmp("[Duration]", szBuff))
			{
				fscanf_s(pFile, "%f", &frm.fDuration);
				break;
			}
		}
		m_vecFrm.push_back(frm);
	}
	

	fclose(pFile);
}

void CAnimation::update()
{
	if (m_bFinish)
		return;

	m_fAccTime += fDT;
	if (m_fAccTime > m_vecFrm[m_iCurFrm].fDuration)
	{
		++m_iCurFrm;
		// 넘어간 시간부터 다시 초기화


		if (m_vecFrm.size() <= m_iCurFrm)
		{
			// 프레임 진행 끝에 도달한 경우
			m_iCurFrm = -1; //-1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}
		m_fAccTime = m_fAccTime - m_vecFrm[m_iCurFrm].fDuration;
	}
		
}


void CAnimation::render(HDC _dc)
{
	if (m_bFinish)
		return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();
	m_vecFrm[m_iCurFrm].vOffset;		// offset
	vPos += m_vecFrm[m_iCurFrm].vOffset;// Object Position에 Offset만큼 추가 이동위치
	//m_vecFrm[m_iCurFrm].vSlice;
	

	// 카메라 렌더링 좌표로 변환
	vPos = CCamera::GetInst()->GetRenderPos(vPos);
	
	/*TransparentBlt(_dc
		,(int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f)
		,(int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.y / 2.f)
		,(int)(m_vecFrm[m_iCurFrm].vSlice.x)
		,(int)(m_vecFrm[m_iCurFrm].vSlice.y)
		,m_pTex->GetDC()
		,(int)(m_vecFrm[m_iCurFrm].vLT.x)
		,(int)(m_vecFrm[m_iCurFrm].vLT.y)
		,(int)(m_vecFrm[m_iCurFrm].vSlice.x)
		,(int)(m_vecFrm[m_iCurFrm].vSlice.y)
		,RGB(255,0,255));*/


	TransparentBlt(_dc
		, (int)(vPos.x - pObj->GetScale().x/2)
		, (int)(vPos.y - pObj->GetScale().y/2)
		, (int)(pObj->GetScale().x)
		, (int)(pObj->GetScale().y)
		, m_pTex->GetDC()
		, (int)(m_vecFrm[m_iCurFrm].vLT.x)
		, (int)(m_vecFrm[m_iCurFrm].vLT.y)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.x)
		, (int)(m_vecFrm[m_iCurFrm].vSlice.y)
		, RGB(255, 0, 255));
	


}
void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float fDuration, UINT _iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT i = 0; i < _iFrameCount; ++i)
	{
		frm.fDuration = fDuration;	// 프레임 지속시간 = 한 사이클
		frm.vSlice = _vSliceSize;	// 자를 위치
		frm.vLT = _vLT + _vStep * (float)i;// 이동할 간격

		// 프레임 정보를 m_vecFrm에 pushback
		m_vecFrm.push_back(frm);
	}
}




