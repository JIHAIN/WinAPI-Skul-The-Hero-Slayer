#pragma once
#include "CState.h"
class Attack1_State :
	public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Attack1_State);
    Attack1_State();
    ~Attack1_State();
};

