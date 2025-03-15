#pragma once
#include "CState.h"
class Mage_Idle_State :
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
    CLONE(Mage_Idle_State);
    Mage_Idle_State();
    ~Mage_Idle_State();
};

