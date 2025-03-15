#pragma once
#include "CState.h"
class Ent_Trace_State :
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
    CLONE(Ent_Trace_State);
    Ent_Trace_State();
    ~Ent_Trace_State();
};

