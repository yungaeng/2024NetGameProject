#pragma once
#include "global.h"
#include "CMonFactory.h"
// 만약 #include "CObject.h" 를 했다면, CObject.h가 변경될 때 마다 컴파일을 했을 것이다.
// 전방선언을 통해 헤더가 헤더를 참조하는 것을 방지
// 하지만 이 경우 실제 타입에 대한 구체적인 정보가 없기 때문에 포인터 타입으로만 사용가능
class CObject;

class CScene
{
private:
	// 자식 포인터들은 부모 포인터로 다 가리킬 수 있다.
	vector<CObject*>	m_arrObj[(UINT)GROUP_TYPE::END];	// 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언
	wstring				m_strName;	//Scene 이름

	UINT				m_iTileX;	// 타일의 x 개수
	UINT				m_iTileY;	// 타일의 y 개수

	UINT				m_iBlockX;
	UINT				m_iBlockY;

	CObject*			m_pPlayer;	// 현재 씬의 플레이어
public:
	// 이름 세팅 및 반환 함수
	void SetName(const wstring& _strName)	{	m_strName = _strName;	}
	const wstring& GetName() { return m_strName; }
	// x축 타일개수 반환
	UINT GetTileX() { return m_iTileX; }	
	// y축 타일개수 반환
	UINT GetTileY() { return m_iTileY; }	
	CObject* GetPlayer() { return m_pPlayer; }

	virtual void start();
	virtual void update();
	virtual void finalupdate();
	virtual void render(HDC _dc);
	virtual void render_tile(HDC _dc);

	// 순수 가상 함수
	// ★★★순수 가상 함수를 보유하고 있는 클래스는 실제 객체화 할 수 있는 클래스가 아닌,
	// 오로지 상속을 통하고 구현해야할 함수의 목록을 알리는 목적이 큰 클래스
	virtual void Enter() = 0;	// 해당 Scene에 진입 시 호출
	virtual void Exit() = 0;	// 해당 Scene을 탈출 시 호출


public:
	// 인라인으로 제작해서 protected를 통해 m_arrObj에 바로 접근하는 것과 동일한 성능 보장
	void AddObject(CObject* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}

	void RegisterPlayer(CObject* _pPlayer) { m_pPlayer = _pPlayer; }

	// 그냥 반환하면 복사해서 반환하기 때문에 복사비용이 추가됨, 따라서 원본 자체를 준다but 수정불가const
	const vector<CObject*>& GetGroupObject(GROUP_TYPE _eType){	return m_arrObj[(UINT)_eType];	}

	// UI오브젝트 그룹 전용 오브젝트 그룹 반환 함수
	vector<CObject*>& GetUIGroup() { return m_arrObj[(UINT)GROUP_TYPE::UI]; }

	void DeleteGroup(GROUP_TYPE _eTarget);
	void DeleteAll();

	void CreateTile(UINT _iXCount, UINT _iYCount);
	void CreateBlock(int _count);
	void LoadTile(const wstring& _strRelativePath);

public:
	CScene();
	virtual ~CScene();
	// ★★★ 상속을 목적으로 하는 클래스의 경우 소멸자에 가상함수를 무조건 사용해야 한다.
	// 씬을 상속하는 자식 객체 전부 소멸할 건데 부모 타입 포인터로 가리키고 있었으니까
	// virtual소멸자를 호출하지 않으면 부모 소멸자만 호출되고 끝남
	// ★★★ 따라서 각각 객체의 소멸자가 호출될 수 있도록 SCene의 가상 소멸자를 만들어야함
};

