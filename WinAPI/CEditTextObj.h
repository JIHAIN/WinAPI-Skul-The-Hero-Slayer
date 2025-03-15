#pragma once
#include "CObj.h"

class CTexture;
class CAnimator;

class CEditTextObj :
    public CObj
{
public:
    CTexture*  m_Texture;

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }

    virtual void Tick() override { int a = 0; }
    virtual void Render() override;
    
public:
    CLONE(CEditTextObj);
    CEditTextObj();
    CEditTextObj(const CEditTextObj& _Other);
    ~CEditTextObj();
};

