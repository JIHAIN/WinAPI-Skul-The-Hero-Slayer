#pragma once
#include "CObj.h"


class CForce :
    public CObj
{
private:
    float   m_Force;        // 힘의 크기
    float   m_Range;        // 힘이 적용되는 범위
    float   m_Duration;     // 힘의 라이프
    float   m_AccTime;      // 라이프 체크용

public:
    void SetForce(float _ForceScale, float _Range)
    {
        m_Force = _ForceScale;
        m_Range = _Range;
    }

    void SetLifeTime(float _time) { m_Duration = _time; }


public:
    virtual void Tick() override;
    virtual void Render() override {}

public:
    CLONE(CForce);
    CForce();
    CForce(const CForce& _Other);
    ~CForce();
};

