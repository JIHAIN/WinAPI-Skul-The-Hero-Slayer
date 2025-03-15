#pragma once
#include "CState.h"
class BigEnt_Trace_State :
    public CState
{

private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BigEnt_Trace_State);
    BigEnt_Trace_State();
    ~BigEnt_Trace_State();
};

