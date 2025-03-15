#pragma once
#include "CState.h"
class BigEnt_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(BigEnt_Hit_State);
    BigEnt_Hit_State();
    ~BigEnt_Hit_State();
};

