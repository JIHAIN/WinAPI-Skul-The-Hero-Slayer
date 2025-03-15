#pragma once
#include "CState.h"
class BEnt_Idle_State :
    public CState
{
private:
    bool MoveOn;
    bool LeftEnd;
    bool RightEnd;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(BEnt_Idle_State);
    BEnt_Idle_State();
    ~BEnt_Idle_State();
};

