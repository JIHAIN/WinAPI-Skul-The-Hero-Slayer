#pragma once
#include "CState.h"
class Balling_State :
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
    CLONE(Balling_State);
    Balling_State();
    ~Balling_State();
};

