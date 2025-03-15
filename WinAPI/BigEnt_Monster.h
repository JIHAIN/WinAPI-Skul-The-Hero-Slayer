#pragma once
#include "CMonster.h"
class BigEnt_Monster :
    public CMonster
{

public:

    CLONE(BigEnt_Monster);
    BigEnt_Monster();
    BigEnt_Monster(const BigEnt_Monster& _Other);
    ~BigEnt_Monster();
};
