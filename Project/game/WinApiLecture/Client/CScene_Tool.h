#pragma once
#include "CScene.h"
class CUI;

class CScene_Tool :
    public CScene
{
private:
    CUI* m_pUI;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();



    // 웬만하면 이넘클래스로 분기처리를 해서 여러가지 동작을 할 수 있도록 ㄱㄱ
public:
    void SetTileIdx();
    void SaveTileData();

    void SaveTile(const wstring& _strFilePath);
    void LoadTileData();

public:
    CScene_Tool();
    ~CScene_Tool();
};

