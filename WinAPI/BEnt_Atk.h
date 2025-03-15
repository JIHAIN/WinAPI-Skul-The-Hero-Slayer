#pragma once
#include "CAttack.h"
class BEnt_Atk :
    public CAttack
{

public:
    CLONE(BEnt_Atk);
    BEnt_Atk();
    BEnt_Atk(const BEnt_Atk& _Other);
    ~BEnt_Atk();
};

