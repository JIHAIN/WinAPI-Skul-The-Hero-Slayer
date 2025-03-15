#pragma once
#include "CObj.h"

class CCollider;
class CAnimator;

class Boss_Door :
    public CObj
{
private:
    CCollider* m_Collider;
    CAnimator* m_Animator;
    bool       Door_On;

public:


public:
    virtual void Tick() override {};
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:
    CLONE(Boss_Door);
    Boss_Door();
    Boss_Door(const Boss_Door& _Other);
    ~Boss_Door();
};
