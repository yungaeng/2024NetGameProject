#include "pch.h"
#include "CScene_Tool.h"
#include "CKeyMgr.h"
#include "CTile.h"

#include "CCore.h"
#include "CResMgr.h"
#include "CSceneMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"

#include "CUIMgr.h"
#include "resource.h"
#include "CPanelUI.h"
#include "CBtnUI.h"

CScene_Tool::CScene_Tool()
	:m_pUI(nullptr)

{
}

CScene_Tool::~CScene_Tool()
{
}


void ChangeScene(DWORD_PTR, DWORD_PTR);

void CScene_Tool::Enter()
{
	// 툴 Scene에서 사용할 메뉴를 붙여준다.
//	
	CCore::GetInst()->DockMenu();
	

	// 타일 생성 - 진입 시
	CreateTile(5, 5);

	// UI 만들어 보기
	Vec2 vResolution = CCore::GetInst()->GetResolution();

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI");
	pPanelUI->SetScale(Vec2(300.f,150.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));
	


	CBtnUI* pBtnUI = new CBtnUI;
	pBtnUI->SetName(L"ChildUI");
	pBtnUI->SetScale(Vec2(100.f, 40.f));
	pBtnUI->SetPos(Vec2(0.f, 0.f));

	// 객체의 멤버함수 포인터 사용하기......................................
	((CBtnUI*)pBtnUI)->SetClickedCallBack(this,(SCENE_MEMFUNC)&CScene_Tool::SaveTileData);
	pPanelUI->AddChild(pBtnUI);

	AddObject(pPanelUI, GROUP_TYPE::UI);

	// 복사본 UI
	/*CUI* pClonePanel = pPanelUI->Clone();
	pClonePanel->SetPos(pClonePanel->GetPos() + Vec2(-300.f, 0.f));
	// 전역 멤버함수 포인터 사용법 ★★★★★★★
	((CBtnUI*)pClonePanel->GetChildUI()[0])->SetClickedCallBack(ChangeScene, 0, 0);
	AddObject(pClonePanel, GROUP_TYPE::UI);

	m_pUI = pClonePanel;*/

	// 카메라 LookAt 지정
	CCamera::GetInst()->SetLookAt(vResolution / 2.f);

}

void CScene_Tool::Exit()
{
	CCore::GetInst()->DivideMenu();
	DeleteAll();
}


void CScene_Tool::update()
{
	CScene::update();
	// 이럼 오버라이딩 왜함?
	// + @를 할 거다.
	SetTileIdx();

	//if (KEY_TAP(KEY::LSHIFT))
	//{
	////	CUIMgr::GetInst()->SetFocusdeUI(m_pUI);
	//	SaveTileData();
	//
	//}
	if (KEY_TAP(KEY::CTRL))
	{
		LoadTileData();
	}
}

void CScene_Tool::SetTileIdx()
{
	

	if (KEY_TAP(KEY::LBTN))
	{
		Vec2 vMousePos = MOUSE_POS;
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);

		int iTileX = (int)GetTileX();
		int iTileY = (int)GetTileY();

		int iCol = (int)vMousePos.x / TILE_SIZE;		// 몇번째 열인지 파악 가능 
		int iRow = (int)vMousePos.y / TILE_SIZE;		// 몇번째 행인지 파악 가능

		// 마우스 위치.x가 음수가 아닌 경우 || 열의 개수를 넘어간 경우
		// || 마우스 위치.y가 음수가 아닌 경우 || 행의 개수를 넘어간 경우 return
		if (vMousePos.x < 0 || iTileX <= iCol
			||vMousePos.y < 0 || iTileY <= iRow)
		{
			return;
		}
		// 인덱스는 마우스 행의 개수 * 최대 열의 개수 + 마우스 열의 개수
		// 이 인덱스가 바로 클릭한 타일의 인덱스
		UINT iIdx = iRow * iTileX + iCol;

		const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

		// 마우스가 클릭한 타일 찾기 완료, 다운 캐스팅 (타일이라고 확정할 수 있음)
		((CTile*)vecTile[iIdx])->AddImgIdx();


	}
}

void CScene_Tool::SaveTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	// 인터넷에 찾아보면 형식을 찾아서 쓸 수 있다.
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;	// 필터로 사용할 설정의 인덱스 0 = ALL, 1 = Tile
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Modal 모든 포커싱을 가져가서 while 돌면서 계속 실행되는 중
	if (GetSaveFileName(&ofn))
	{
		// 저장버튼을 누르면 true
		// -> szName에 경로를 저장해줌 그 경로를 인자로 넣어서 진짜 저장하기
	
		SaveTile(szName);
	}
	// 취소 버튼을 누르면 false
}

void CScene_Tool::SaveTile(const wstring& _strFilePath)
{

	// 커널 오브젝트
	FILE* pFile = nullptr;
	// 2바이트 전용 파일 오픈 
	_wfopen_s(&pFile, _strFilePath.c_str(),L"wb"); // L"w" -> write모드, 저장장치에서 불러들이는 건 불가능하다.

	assert(pFile);
	
	// 데이터 저장 - 타일 가로 세로 개수
	UINT xCount = GetTileX();
	UINT yCount = GetTileY();

	// 파일에 데이터 저장하는 방법 : 첫인자로 저장될 데이터의 시작주소를 void 포인터로 받고,
	// 이후 인자 2에 그 데이터의 사이즈를 받아 어떻게 읽을지 결정한다. 
	// 인자 3은 배열일 경우 개수를 적어준다.
	// 인자 4는 스트림을 적음
	fwrite(&xCount, sizeof(UINT), 1, pFile);
	fwrite(&yCount, sizeof(UINT), 1, pFile);

	// 모든 타일들을 개별적으로 저장할 데이터를 저장하게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		// 각자 타일의 정보를 저장시키기
		((CTile*)vecTile[i])->Save(pFile);
	}

	fclose(pFile);
}

void CScene_Tool::LoadTileData()
{
	OPENFILENAME ofn = {};

	wchar_t szName[256] = {};

	// 인터넷에 찾아보면 형식을 찾아서 쓸 수 있다.
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
	ofn.lpstrFile = szName;
	ofn.nMaxFile = sizeof(szName);
	ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
	ofn.nFilterIndex = 0;	// 필터로 사용할 설정의 인덱스 0 = ALL, 1 = Tile
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;

	wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
	strTileFolder += L"tile";
	ofn.lpstrInitialDir = strTileFolder.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Modal 모든 포커싱을 가져가서 while 돌면서 계속 실행되는 중
	if (GetOpenFileName(&ofn))
	{
		// 저장버튼을 누르면 true
		// -> szName에 경로를 저장해줌 그 경로를 통해 상대 경로 구해 넣어서 진짜 읽어오기
		wstring strRelativePath = CPathMgr::GetInst()->GetRelativePath(szName);
		LoadTile(strRelativePath);
	}
	// 취소 버튼을 누르면 false
}


// 전역 함수
void ChangeScene(DWORD_PTR, DWORD_PTR)
{
	ChangeScene(SCENE_TYPE::START);
}





// !!주의 Cscene_tool의 멤버함수가 아닌 전역함수이다.★★★★★★★
/// ======================
// Tile Count Window Proc
/// ======================
// CALLBACK  - 함수 호출 규약에 관련한 것 
INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK )
		{
			// 다이어로그에 있는 아이템(적힌 문자열)을 숫자로 변환하여 가져온다.
			// OK 버튼이 눌린 경우의 처리할 일 - hWnd, ID, ? , 음수 양수 signed
			UINT iXCount = GetDlgItemInt(hDlg,IDC_EDIT1,nullptr,false);	
			UINT iYCount = GetDlgItemInt(hDlg, IDC_EDIT2, nullptr, false);

		
			// 기존에 있던 타일 삭제...~ ★주의 멤버 함수가 아니므로 현재 씬이 무엇인지 모른다.
			CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
			// 이 기능은 Tool Scene에서만 사용 가능해야 한다.
			// 따라서 다운캐스팅을 CScene_Tool로 실행해 성공한 경우에만 실행한다.
			CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
			assert(pToolScene);	// 다운 캐스팅 성공 실패 확인 - nullptr인 경우 어설트 실행
			
			// 타일 그룹 오브젝트들 삭제
			pToolScene->DeleteGroup(GROUP_TYPE::TILE);

			// 타일 생성
			pToolScene->CreateTile(iXCount, iYCount);


			// EndDialog - 해당 윈도우가 종료됨을 알림 - 두번째 인자로 전달해주는 LOWORD(wParam)(INT_PTR)
			// 눌린 본인의 아이디 값을 준다 - DilogBox의 리턴 값으로 눌린 버튼이 뭔지 알려줄 수 있다.
			// but 그냥 여기서 눌린 버튼에 따라 할 일을 바로 작성해준다 - 일관성
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			// 캔슬인 경우 그냥 종료
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}