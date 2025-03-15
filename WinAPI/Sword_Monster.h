#pragma once
#include "CMonster.h"

class CStateMachine;


class Sword_Monster :
    public CMonster
{
private:

    CStateMachine* pSM;

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    CLONE(Sword_Monster);
    Sword_Monster();
    Sword_Monster(const Sword_Monster& _Other);
    ~Sword_Monster();

};

