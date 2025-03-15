#pragma once
#include "CObj.h"

class CCollider;

class Monster_Wall :
    public CObj
{
private:
    CCollider* m_Collider;



public:
    virtual void SetScale(Vec2 _Scale) override;
    virtual void SetScale(float _Width, float _Height) override;
    virtual void SetScaleMouse(Vec2 _Scale);

public:
    virtual void Tick() override;
    virtual void Render() override {}
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(Monster_Wall);
    Monster_Wall();
    Monster_Wall(const Monster_Wall& _Other);
    ~Monster_Wall();
};

