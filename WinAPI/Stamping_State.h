#pragma once
#include "CState.h"
class Stamping_State :
    public CState
{
private:
    bool LeftHand;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Stamping_State);
    Stamping_State();
    ~Stamping_State();
};

