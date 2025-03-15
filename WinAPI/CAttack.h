#pragma once
#include "CObj.h"

class CAnimator;
class CCollider;

class CAttack :
    public CObj
{
protected:
    CAnimator* m_Animator;
    CCollider* m_Collider;

    float      m_Duration;
    float      m_Acctime;

public:
    virtual void SetCollider(float _X, float _Y);

    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CAttack);
    CAttack();
    CAttack(const CAttack& _Other);
    ~CAttack();
};

