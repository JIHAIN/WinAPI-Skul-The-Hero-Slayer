#pragma once
#include "CState.h"
class Right_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Right_State);
    Right_State();
    ~Right_State();
};

