#pragma once
#include "CState.h"

class Mage_Atk;

class Mage_Attack_State :
    public CState
{
public:
    Mage_Atk* Fireball;

    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Mage_Attack_State);
    Mage_Attack_State();
    ~Mage_Attack_State();
};

