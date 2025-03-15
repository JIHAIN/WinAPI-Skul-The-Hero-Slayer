#pragma once
#include "CState.h"
class BigEnt_Attack_State :
    public CState
{

public:
    int fistatk;

    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BigEnt_Attack_State);
    BigEnt_Attack_State();
    ~BigEnt_Attack_State();
};

