#pragma once
#include "CState.h"
class Sword_Idle_State :
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
    CLONE(Sword_Idle_State);
    Sword_Idle_State();
    ~Sword_Idle_State();
};

