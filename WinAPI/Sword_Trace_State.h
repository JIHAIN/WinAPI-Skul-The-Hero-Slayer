#pragma once
#include "CState.h"
class Sword_Trace_State :
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
    CLONE(Sword_Trace_State);
    Sword_Trace_State();
    ~Sword_Trace_State();
};

