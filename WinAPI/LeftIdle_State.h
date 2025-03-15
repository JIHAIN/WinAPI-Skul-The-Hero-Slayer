#pragma once
#include "CState.h"
class LeftIdle_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(LeftIdle_State);
    LeftIdle_State();
    ~LeftIdle_State();
};

