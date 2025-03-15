#pragma once
#include "CState.h"

class Sniper_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(Sniper_Hit_State);
    Sniper_Hit_State();
    ~Sniper_Hit_State();
};