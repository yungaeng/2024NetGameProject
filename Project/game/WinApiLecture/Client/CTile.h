#pragma once
#include "CObject.h"

class CTexture;
class CTile :
    public CObject
{

private:
    CTexture*   m_pTileTex; // 타일 텍스처
    int         m_iImgIdx;  // 출력할 타일 번호 - 음수인 경우 아무것도 참조하지 않겠다
private:
    virtual void update();
    virtual void render(HDC _dc);

public:
    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFIle);

    
public:
    void SetTexture(CTexture* _pTex) { m_pTileTex = _pTex; }

    void AddImgIdx()
    {
        ++m_iImgIdx;
    }

    CLONE(CTile);
public:
    CTile();
    ~CTile();
};

