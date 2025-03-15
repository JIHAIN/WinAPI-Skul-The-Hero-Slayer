#pragma once
#include "CAttack.h"

class CAnimator;
class CCollider;

class Dark_Waves :
    public CObj
{
protected:
    CAnimator* m_Animator;
    CCollider* m_Collider;

    float      m_Duration;
    float      m_Acctime;
    float      m_Dir;

public:
    virtual void SetCollider(float _X, float _Y);

    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(Dark_Waves);
    Dark_Waves(bool _Right);
    Dark_Waves(const Dark_Waves& _Other);
    ~Dark_Waves();
};