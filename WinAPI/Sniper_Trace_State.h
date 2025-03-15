#pragma once
#include "CState.h"
class Sniper_Trace_State :
    public CState
{
private:
    Vec2 m_InitPos; // 초기 위치
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Sniper_Trace_State);
    Sniper_Trace_State();
    ~Sniper_Trace_State();
};

