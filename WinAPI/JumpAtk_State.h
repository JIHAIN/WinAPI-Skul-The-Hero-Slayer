#pragma once
#include "CState.h"
class JumpAtk_State :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(JumpAtk_State);
    JumpAtk_State();
    ~JumpAtk_State();
};

