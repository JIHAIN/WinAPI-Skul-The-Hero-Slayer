#pragma once
#include "CState.h"

class Sword_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(Sword_Hit_State);
    Sword_Hit_State();
    ~Sword_Hit_State();
};