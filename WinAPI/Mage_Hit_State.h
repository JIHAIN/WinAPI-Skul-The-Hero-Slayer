#pragma once
#include "CState.h"

class Mage_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(Mage_Hit_State);
    Mage_Hit_State();
    ~Mage_Hit_State();
};