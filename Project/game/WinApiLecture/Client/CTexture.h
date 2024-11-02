#pragma once
#include "CRes.h"
class CTexture :
    public CRes
{

private:
    HDC         m_dc;
    HBITMAP     m_hBit;
    BITMAP      m_bitInfo;

public:
    void Load(const wstring& _strFilePath);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }

    // 리소스 매니저 이외에 텍스처를 동적할당 할 수 없도록 private선언, friend class 등록
private:
    CTexture();
    ~CTexture();

    friend class CResMgr;
};

