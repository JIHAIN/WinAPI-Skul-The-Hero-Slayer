#pragma once
#include "CObj.h"

class CAnimator;

class CEditAniObj :
    public CObj
{
private:
    CAnimator* m_Animator;


public:
    CAnimator* GetEditAnimator() { return m_Animator; }

    virtual void Tick() override { int a = 0; }
    virtual void Render() override;

public:
    CLONE(CEditAniObj);
    CEditAniObj();
    CEditAniObj(const CEditAniObj& _Other);
    ~CEditAniObj();
};


