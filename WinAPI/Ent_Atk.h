#pragma once
#include "CAttack.h"

class Ent_Atk :
    public CAttack
{

public:
    CLONE(Ent_Atk);
    Ent_Atk();
    Ent_Atk(const Ent_Atk& _Other);
    ~Ent_Atk();
};

