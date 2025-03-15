#pragma once
#include "CObj.h"

class CAnimator;

class CSkullHead :
    public CObj
{
private:
    Vec2        m_Direction;
    CCollider*  m_Collider;
    CAnimator*  m_Animator;

    float       m_Speed;
    float       m_MaxDistance;
    float       m_TraveledDistance;
    bool        m_IsGrounded;
    bool        m_IsFalling;
    float       m_LastTeleportTime;
    float       m_FallSpeed;

public:
    CLONE(CSkullHead);
    CSkullHead();
    ~CSkullHead();

    virtual void Tick() override;
    virtual void Render() override;
    void SetDirection(Vec2 dir) { m_Direction = dir; m_Direction.Normalize(); }
    void SetSpeed(float speed) { m_Speed = speed; }
    void SetMaxDistance(float dist) { m_MaxDistance = dist; }
    bool IsGrounded() const { return m_IsGrounded; }
    Vec2 GetPosition() { return GetPos(); }
    CAnimator* GetAnimaor() { return m_Animator; }
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    void Teleport(Vec2 newPosition);
};