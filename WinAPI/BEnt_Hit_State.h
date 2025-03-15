#pragma once
#include "CState.h"

class BEnt_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(BEnt_Hit_State);
    BEnt_Hit_State();
    ~BEnt_Hit_State();
};