#pragma once
#include "CAttack.h"
class BigEnt_Atk :
    public CAttack
{
public:
    CLONE(BigEnt_Atk);
    BigEnt_Atk();
    BigEnt_Atk(const BigEnt_Atk& _Other);
    ~BigEnt_Atk();
};

