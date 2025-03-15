#pragma once
#include "CState.h"
class Jump_State :
	public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Jump_State);
    Jump_State();
    ~Jump_State();
};

