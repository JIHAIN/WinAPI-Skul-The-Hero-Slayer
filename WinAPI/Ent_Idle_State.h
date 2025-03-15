#pragma once
#include "CState.h"
class Ent_Idle_State :
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
    CLONE(Ent_Idle_State);
    Ent_Idle_State();
    ~Ent_Idle_State();
};

