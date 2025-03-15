#pragma once
#include "CState.h"
class Fall_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Fall_State);
    Fall_State();
    ~Fall_State();
};

