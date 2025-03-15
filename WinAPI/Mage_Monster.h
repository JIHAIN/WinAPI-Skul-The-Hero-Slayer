#pragma once
#include "CMonster.h"

class CStateMachine;

class Mage_Monster :
    public CMonster
{

    CStateMachine* pSM;
public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    CLONE(Mage_Monster);
    Mage_Monster();
    Mage_Monster(const Mage_Monster& _Other);
    ~Mage_Monster();
 
};

