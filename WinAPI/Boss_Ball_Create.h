#pragma once
#include "CObj.h"

class CAnimator;

class Boss_Ball_Create :
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
    CLONE(Boss_Ball_Create);
    Boss_Ball_Create(Vec2 _Pos);
    Boss_Ball_Create(const Boss_Ball_Create& _Other);
    ~Boss_Ball_Create();
};

