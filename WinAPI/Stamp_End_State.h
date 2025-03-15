#pragma once
#include "CState.h"
class Stamp_End_State :
    public CState
{
private:
    float AccTime;
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Stamp_End_State);
    Stamp_End_State();
    ~Stamp_End_State();
};

