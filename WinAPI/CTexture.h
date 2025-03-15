#pragma once
#include "CAsset.h"


class CTexture :
    public CAsset
{
private:
    HBITMAP     m_hBitmap; // 비트맵 핸들
    HDC         m_hDC;     // 그걸 가르키는 DC
    BITMAP      m_BitInfo; // 비트맵 정보


public:
    UINT GetWidth() { return m_BitInfo.bmWidth; }
    UINT GetHeight() { return m_BitInfo.bmHeight; }
    HDC GetDC() { return m_hDC; }
    void* GetPixelx() { return m_BitInfo.bmBits; }

private:
    virtual int Load(const wstring& _FilePath) override;
    int Create(UINT _Width, UINT _Height);

public:
    CTexture();
    ~CTexture();

    friend class CAssetMgr;
};

