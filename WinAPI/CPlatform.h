#pragma once
#include "CObj.h"

class CCollider;

class CPlatform :
    public CObj
{
private:
    CCollider*  m_Collider;

    

public:
    virtual void SetScale(Vec2 _Scale) override;
    virtual void SetScale(float _Width, float _Height) override;
    virtual void SetScaleMouse(Vec2 _Scale);

public:
    virtual void Tick() override;
    virtual void Render() override {}
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    Vec2 CalculateCollisionNormal(CCollider* _OwnCollider, CCollider* _OtherCollider); // AI가 짜준 충돌 깊이 계산 코드
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(CPlatform);
    CPlatform();
    CPlatform(const CPlatform& _Other);
    ~CPlatform();
};

