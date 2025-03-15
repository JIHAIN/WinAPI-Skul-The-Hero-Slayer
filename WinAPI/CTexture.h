#pragma once
#include "CAsset.h"


class CTexture :
    public CAsset
{
private:
    HBITMAP     m_hBitmap; // ��Ʈ�� �ڵ�
    HDC         m_hDC;     // �װ� ����Ű�� DC
    BITMAP      m_BitInfo; // ��Ʈ�� ����


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

