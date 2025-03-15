#pragma once
#include "CObj.h"

class CAnimator;

class Effect_Obj :
    public CObj
{
private:
    CAnimator* m_Animator;
    float LifeTime;
    float Duration;


public:
    CAnimator* GetEditAnimator() { return m_Animator; }
    void SetDuration(float _Duration) { Duration = _Duration; }

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Effect_Obj);
    Effect_Obj();
    Effect_Obj(const Effect_Obj& _Other);
    ~Effect_Obj();
};
