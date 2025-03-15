#pragma once
#include "CState.h"
class Mage_Trace_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Mage_Trace_State);
    Mage_Trace_State();
    ~Mage_Trace_State();
};

