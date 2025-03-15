#pragma once
#include "CState.h"
class Attack2_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Attack2_State);
    Attack2_State();
    ~Attack2_State();
};

