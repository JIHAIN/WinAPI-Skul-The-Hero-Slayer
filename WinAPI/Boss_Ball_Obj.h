#pragma once
#include "CObj.h"

class CAnimator;
class Collider;

class Boss_Ball_Obj :
    public CObj
{
private:

    CAnimator*   m_Animator;
    CCollider*   m_Collider;

    float        m_AccTime;
    float        m_DownTime;
    bool         Sound1;

public:
    virtual void SetCollider(float _X, float _Y);

    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:
    CLONE(Boss_Ball_Obj);
    Boss_Ball_Obj();
    Boss_Ball_Obj(const Boss_Ball_Obj& _Other);
    ~Boss_Ball_Obj();
};

