#include "pch.h"
#include "func.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "random"

using std::mt19937;
using std::random_device;
using std::uniform_int_distribution;

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CREATE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;
	evn.wParam = (DWORD_PTR)_eGroup;	// enum값은 4바이트 정수

	CEventMgr::GetInst()->AddEvent(evn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::DELETE_OBJECT;
	evn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::SCENE_CHANGE;
	evn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInst()->AddEvent(evn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent evn = {};
	evn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)_pAI;
	evn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInst()->AddEvent(evn);
}

void FScanf(char* _pOutBuff, FILE* _pFile)
{
	int i = 0;
	while (true)
	{
		char c = (char)getc(_pFile);
		if (c == '\n')
		{
			_pOutBuff[i++] = '\0';
			break;
		}
		_pOutBuff[i++] = c;
	}
}

void SaveWString(const wstring& _str, FILE* _pFile)
{
	
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	// 문자 길이 저장
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	// 문자열 저장
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWString(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	// 파일로부터 문자열의 길이를 읽어옴
	fread(&iLen, sizeof(size_t), 1, _pFile);

	// 뒤 이어서 pFile이 다음 데이터를 가리키고 있기 때문에 iLen 만큼 읽어오면 됨

	wchar_t szBuff[256] = {};		// 문자열을 받아올 버퍼 생성

	// 바로 _str(wstring)에 넣지 않는 이유는, _str은 가변 배열이기 때문에 얼마나 할당되어있는지 모름
	// 바로 크기를 넘어서는 문자열을 넣어주면 오류 발생 가능
	// 파일로부터 문자를 iLen만큼 가져옴
	fread(szBuff, sizeof(wchar_t), iLen, _pFile);

	// = operator에 가변 배열 기능을 구현해놨을 것이다.
	_str = szBuff;
}


int MakeRandomInt(int start, int end)
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> id(start, end);

	return id(mt);
}
