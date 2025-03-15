#pragma once
#include "CObj.h"
#include "CAssetMgr.h"

class CTexture;


class CBackground :
    public CObj
{
private:
    CTexture* m_Texture;
    int       IDX;

public:
    void SetTexture(const wstring& _Key, const wstring& _Path) { m_Texture = CAssetMgr::Get()->LoadTexture(_Key, _Path); }
    void SetIDX(int _Idx) { IDX = _Idx; }

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(CBackground);
    CBackground();
    ~CBackground();
};

