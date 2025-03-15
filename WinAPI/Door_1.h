#pragma once
#include "CObj.h"
class CCollider;
class CAnimator;

class Door_1 :
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
    CLONE(Door_1);
    Door_1();
    Door_1(const Door_1& _Other);
    ~Door_1();
};