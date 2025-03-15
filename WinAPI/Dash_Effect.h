#pragma once
#include "CObj.h"
class CAnimator;

class Dash_Effect :
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
    CLONE(Dash_Effect);
    Dash_Effect(const Dash_Effect& _Other);
    Dash_Effect(bool _Left);
    ~Dash_Effect();
};