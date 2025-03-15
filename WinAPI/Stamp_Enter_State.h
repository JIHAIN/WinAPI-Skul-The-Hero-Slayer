#pragma once
#include "CState.h"
class Stamp_Enter_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Stamp_Enter_State);
    Stamp_Enter_State();
    ~Stamp_Enter_State();
};

