#pragma once
#include "CState.h"
class Sniper_Idle_State :
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
    CLONE(Sniper_Idle_State);
    Sniper_Idle_State();
    ~Sniper_Idle_State();
};

