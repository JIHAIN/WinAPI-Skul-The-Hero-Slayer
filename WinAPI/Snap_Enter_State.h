#pragma once
#include "CState.h"
class Snap_Enter_State :
    public CState
{
private:
    bool first;
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Snap_Enter_State);
    Snap_Enter_State();
    ~Snap_Enter_State();
};

