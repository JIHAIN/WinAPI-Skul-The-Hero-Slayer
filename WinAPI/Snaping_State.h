#pragma once
#include "CState.h"

class Effect_Obj;

class Snaping_State :
    public CState
{
private:
    bool Left_Hend_Atk;
    bool Ani_One;

    Effect_Obj* effect;
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Snaping_State);
    Snaping_State();
    ~Snaping_State();
};

