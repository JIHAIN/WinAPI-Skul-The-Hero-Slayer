#pragma once
#include "CState.h"
class Upfall_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Upfall_State);
    Upfall_State();
    ~Upfall_State();
};

