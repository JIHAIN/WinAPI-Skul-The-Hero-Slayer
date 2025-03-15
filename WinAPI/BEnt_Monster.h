#pragma once
#include "CMonster.h"

class CStateMachine;

class BEnt_Monster :
    public CMonster
{
    CStateMachine* pSM;
public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

    CLONE(BEnt_Monster);
    BEnt_Monster();
    BEnt_Monster(const BEnt_Monster& _Other);
    ~BEnt_Monster();
 
};

