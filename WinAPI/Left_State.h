#pragma once
#include "CState.h"
class Left_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Left_State);
    Left_State();
    ~Left_State();
};

