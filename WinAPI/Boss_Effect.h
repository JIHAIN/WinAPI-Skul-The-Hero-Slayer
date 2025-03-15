#pragma once
#include "CObj.h"

class CAnimator;

class Boss_Effect :
    public CObj
{
private:
    CAnimator* m_Animator;
    float LifeTime;


public:
    CAnimator* GetEditAnimator() { return m_Animator; }

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Boss_Effect);
    Boss_Effect();
    Boss_Effect(const Boss_Effect& _Other);
    ~Boss_Effect();
};
