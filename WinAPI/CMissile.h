#pragma once
#include "CObj.h"

class CMissile :
    public CObj
{
private:
    Vec2    m_Velocity;
    float   m_Mass;

public:
    void SetVelocity(Vec2 _vDir, float _Speed)
    {
        m_Velocity = _vDir;
        m_Velocity.Normalize();
        m_Velocity *= _Speed;
    }

    void AddVelocity(Vec2 _Accel)
    {
        m_Velocity += _Accel;

        if (m_Velocity.Length() > 1000.f)
        {
            m_Velocity.Normalize();
            m_Velocity *= 1000.f;
        }
    }

    Vec2 GetVelocity() { return m_Velocity; }

    float GetMass() { return m_Mass; }

public:
    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:
    CLONE(CMissile);
    CMissile();
    ~CMissile();
};

