#pragma once
#include "CState.h"
class Ent_Hit_State :
    public CState
{
private:
    float AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE(Ent_Hit_State);
    Ent_Hit_State();
    ~Ent_Hit_State();
};