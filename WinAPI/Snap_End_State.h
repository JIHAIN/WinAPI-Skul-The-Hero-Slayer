#pragma once
#include "CState.h"
class Snap_End_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Snap_End_State);
    Snap_End_State();
    ~Snap_End_State();
};

