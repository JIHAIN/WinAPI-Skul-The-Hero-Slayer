#pragma once
#include "CState.h"
class Boss_Idle :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(Boss_Idle);
    Boss_Idle();
    ~Boss_Idle();
};

