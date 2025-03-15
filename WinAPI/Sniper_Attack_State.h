#pragma once
#include "CState.h"
class Sniper_Attack_State :
    public CState
{

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Sniper_Attack_State);
    Sniper_Attack_State();
    ~Sniper_Attack_State();
};

