#pragma once
#include "CObj.h"

#include "CCollider.h"

class Cfloor :
    public CObj
{
private:
    CCollider* m_Collider;


public:
    virtual void SetScale(Vec2 _Scale) override;
    virtual void SetScale(float _Width, float _Height) override;

public:
    virtual void Tick() override;
    virtual void Render() override {};
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(Cfloor);
    Cfloor();
    Cfloor(const Cfloor& _Other);
    ~Cfloor();
};

