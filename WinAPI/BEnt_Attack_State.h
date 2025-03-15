#pragma once
#include "CState.h"
class BEnt_Attack_State :
    public CState
{
private:
    float Cooltime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BEnt_Attack_State);
    BEnt_Attack_State();
    ~BEnt_Attack_State();
};

