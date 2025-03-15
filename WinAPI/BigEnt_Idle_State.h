#pragma once
#include "CState.h"
class BigEnt_Idle_State :
    public CState
{
private:
    bool MoveOn;
    bool LeftEnd;
    bool RightEnd;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BigEnt_Idle_State);
    BigEnt_Idle_State();
    ~BigEnt_Idle_State();
};

