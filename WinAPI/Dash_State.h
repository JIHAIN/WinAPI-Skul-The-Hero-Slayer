#pragma once
#include "CState.h"
class Dash_State :
	public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Dash_State);
    Dash_State();
    ~Dash_State();
};

