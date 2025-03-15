#pragma once
#include "CState.h"

class Dark_Wave_State :
    public CState
{
public:
    float AccTime;
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Dark_Wave_State);
    Dark_Wave_State();
    ~Dark_Wave_State();
};