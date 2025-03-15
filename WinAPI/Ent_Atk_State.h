#pragma once
#include "CState.h"
class Ent_Atk_State :
    public CState
{

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Ent_Atk_State);
    Ent_Atk_State();
    ~Ent_Atk_State();
};

