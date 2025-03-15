#pragma once
#include "CState.h"
class Sword_Attack_State :
    public CState
{

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Sword_Attack_State);
    Sword_Attack_State();
    ~Sword_Attack_State();
};

