#pragma once
#include "CState.h"
class RightIdle_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(RightIdle_State);
    RightIdle_State();
    ~RightIdle_State();
};

