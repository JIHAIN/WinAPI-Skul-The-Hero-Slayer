#pragma once
#include "CState.h"
class Attack3_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Attack3_State);
    Attack3_State();
    ~Attack3_State();
};

