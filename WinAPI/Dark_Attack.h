#pragma once
#include "CObj.h"
class CAnimator;
class CCollider;


class Dark_Attack :
    public CObj
{
protected:
    CAnimator* m_Animator;
    CCollider* m_Collider;

   

    float      m_Duration;
    float      m_Acctime;

public:
    virtual void SetCollider(float _X, float _Y);
    CAnimator* GetDarkAnimator() { return m_Animator; }

    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(Dark_Attack);
    Dark_Attack();
    Dark_Attack(const Dark_Attack& _Other);
    ~Dark_Attack();
};