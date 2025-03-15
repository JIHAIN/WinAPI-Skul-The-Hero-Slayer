#pragma once
#include "CState.h"

class Ball_Enter_State :
    public CState
{
private:
    bool BallAtk;
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Ball_Enter_State);
    Ball_Enter_State();
    ~Ball_Enter_State();
};

