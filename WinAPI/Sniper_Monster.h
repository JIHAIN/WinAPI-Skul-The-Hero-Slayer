#pragma once
#include "CMonster.h"

class CStateMachine;

class Sniper_Monster :
    public CMonster
{
private:
    CStateMachine* pSM;

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:

    CLONE(Sniper_Monster);
    Sniper_Monster();
    Sniper_Monster(const Sniper_Monster& _Other);
    ~Sniper_Monster();
  
};

