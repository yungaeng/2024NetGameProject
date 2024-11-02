#include "pch.h"
#include "CCore.h"
#include "CScene.h"
#include "CObject.h"
#include "CTile.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CBlock.h"



CScene::CScene()
	:m_iTileX(0)
	,m_iTileY(0)
	,m_iBlockX(0)
	,m_iBlockY(0)
	,m_pPlayer(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// m_arrObj[i] 그룹 벡터의 j번째 물체 삭제
 			delete m_arrObj[i][j];
		}
	}
	// 벡터또한 CScene의 멤버이므로 벡터의 소멸자가 호출
}


void CScene::start()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			m_arrObj[i][j]->start();
		}
	}
}

void CScene::update()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			if (!m_arrObj[i][j]->IsDead())
			{
				m_arrObj[i][j]->update();
			}
		}
	}
}

// 마무리 업데이트
void CScene::finalupdate()
{
	
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++j)
		{
			// 죽은 물체도 마지막까지 파이널 업데이트 - 충돌 체크 정상화 등
			m_arrObj[i][j]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		// 타일 렌더링은 따로 실행
		// 만약 타일 오브젝트가 데드 처리가 되어야한다면 이터레이터로 다시 빼줘야됨
		if ((UINT)GROUP_TYPE::TILE == i)
		{
			render_tile(_dc);
			continue;
		}



		vector<CObject*>::iterator iter = m_arrObj[i].begin();
		for (; iter != m_arrObj[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->render(_dc);
				++iter;
			}
			else
			{
				// 데드상태인 경우
				// 현재 이터레이터가 가리키는 데이터를 삭제하고 다음 이터레이터를 받아옴
				iter = m_arrObj[i].erase(iter);
			}
		}
	}
}

void CScene::render_tile(HDC _dc)
{

	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	// 카메라가 실제로 보고있는 좌표를 알아오기
	Vec2 vCamLook =  CCamera::GetInst()->GetLookAt();


	// 현재 화면 해상도 알아오기
	Vec2 vRestoultion = CCore::GetInst()->GetResolution();


	// 카메라가 보고있는 곳의 좌상단 좌표
	Vec2 vLeftTop = vCamLook - vRestoultion / 2;

	int iTileSize = TILE_SIZE;	// 타일 사이즈 알아오기

	// 좌상단의 인덱스 구하기
	// x좌표를 타일 사이즈로 나누면 열
	// y좌표를 타일 사이즈로 나누면 행을 구할 수 있음
	int iLTCol = (int)vLeftTop.x / (int)iTileSize;
	int iLTRow = (int)vLeftTop.y / (int)iTileSize;




	int iClientWidth = (int)(vRestoultion.x / iTileSize + 1);		// 한 화면에 나올 타일의 가로 개수
	int iClientheight = (int)(vRestoultion.y / iTileSize + 1);	// 한 화면에 나올 세로 개수

	// 행의 반복횟수 - 화면에 나오는 행의 개수
	for (int iCurRow = iLTRow ; iCurRow < (iLTRow + iClientheight); ++iCurRow)
	{
		// 열의 반복횟수 - 화면에 나오는 열의 개수
		for (int iCurCol = iLTCol; iCurCol < (iLTCol + iClientWidth); ++iCurCol)
		{
			// 타일 가로열의 개수가 음수이거나, 타일의 최대 가로 개수를 넘어서면 안됨 
			if (iCurCol < 0 || m_iTileX <= (unsigned)iCurCol
				// 타일 세로열의 개수가 음수이거나, 타일의 최대 세로 개수를 넘어서면 안됨
				|| iCurRow < 0 || m_iTileY <= (unsigned)iCurRow)
			{
				// 렌더링하지 않는다.
				continue;
			}

			int iIdx = (m_iTileX * iCurRow) + iCurCol;		// 타일 개수 * 행의 개수 + 열의 개수 = 해당 인덱스
			vecTile[iIdx]->render(_dc);
		}
	}


}

// 한 그룹의 모든 오브젝트 삭제하기
void CScene::DeleteGroup(GROUP_TYPE _eTarget)
{
	// ★★★벡터 안에 주소를 동적할당 하여 넣은 경우
	// 벡터를 지울 때는 벡터 안의 주소를 먼저 해제하고, 그 다음 벡터를 해제한다.
	// 템플릿으로 벡터 해제 함수 구현
	// 정석
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eTarget]);

	// < > 무슨 자료형을 넣어서 만들지 정해줘야 한다!!! 밑에거 자체는 함수 템플릿이지 함수가 아니다.
	//Safe_Delete_Vec(m_arrObj[(UINT)_eTarget]);
}

void CScene::DeleteAll()
{
	// 모든 그룹 삭제
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		DeleteGroup((GROUP_TYPE)i);
	}
}

void CScene::CreateTile(UINT _iXCount, UINT _iYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);
	m_iTileX = _iXCount;
	m_iTileY = _iYCount;
	CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"Texture\\Tile.bmp");
	for (UINT i = 0; i < _iYCount; ++i)
	{
		for (UINT j = 0; j < _iXCount; ++j)
		{
			CTile* pTile = new CTile();
			pTile->SetPos(Vec2((float)(j * TILE_SIZE), (float)(i * TILE_SIZE)));
			pTile->SetTexture(pTileTex);
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::CreateBlock(int _count)
{
	// 인버서스 때문에 살짝 건드림
	//DeleteGroup(GROUP_TYPE::TILE);
	/*m_iBlockX = _iXCount;
	m_iBlockY = _iYCount;*/
	//CTexture* pTileTex = CResMgr::GetInst()->LoadTexture(L"Tile", L"Texture\\Tile.bmp");
	for (UINT i = 0; i < (UINT)_count; ++i)
	{
			CBlock* pBlock = new CBlock();

			// 건드림
			pBlock->SetScale(Vec2(40.f, 40.f));
			pBlock->SetName(L"Block");
			pBlock->SetPos(Vec2(MakeRandomInt(20,1260),MakeRandomInt(20, 780)));
			//pTile->SetTexture(pTileTex);

			AddObject(pBlock, GROUP_TYPE::BLOCK);
		
	}
}

void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;
	// 커널 오브젝트
	FILE* pFile = nullptr;
	// 2바이트 전용 파일 오픈 
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb"); // L"r" -> read모드, 저장장치에 저장하는 건 불가능하다.
	// r, w 와 rb , wb의 차이 b(바이너리)를 적어주지 않으면 아스키코드로 파일에 적어줌
	// 사실 별 상관 없어보이지만, 27 = ESC(탈출문자)로 생각하기 때문에
	// 특정 상황에 파일이 닫히는 상황이 발생할 수 있다.
	// 따라서 b를 적어 바이너리 데이터(이진 숫자) 데이터로 생각하겠다는 뜻을 전달.
	// 또한 읽기 모드는 읽으려고 하는 경로에 파일이 없으면 실패함 - write와 다름
	
	assert(pFile);
	// 타일 가로 세로 개수 불러오기
	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	// 불러온 개수에 맞게 EmptyTile 들 만들어두기
	CreateTile(xCount, yCount);

	// 만들어진 타일 개별로 필요한 정보를 불러오게 함
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t i = 0; i < vecTile.size(); ++i)
	{
		// 각자 타일의 정보를 시키기
		((CTile*)vecTile[i])->Load(pFile);
	}

	fclose(pFile);
}
