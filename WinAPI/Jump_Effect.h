#pragma once
#include "CObj.h"
class CAnimator;

class Jump_Effect :
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
    CLONE(Jump_Effect);
    Jump_Effect();
    Jump_Effect(const Jump_Effect& _Other);
    ~Jump_Effect();
};