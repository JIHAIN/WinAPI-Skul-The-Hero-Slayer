#pragma once
#include "CObj.h"

class CCollider;
class CAnimator;

class CPlayer_Normal_Attack :
    public CObj
{
private:
    CAnimator* m_Animator;
    CCollider* m_Collider;

    float      m_Duration;
    float      m_Acctime;

public:
    virtual void Tick() override ;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

    void SetCollider(float _X, float _Y);

public:
    CLONE(CPlayer_Normal_Attack);
    CPlayer_Normal_Attack();
    ~CPlayer_Normal_Attack();
};

