#pragma once
#include "CMonster.h"

class CStateMachine;


class Ent_Monster :
    public CMonster
{
    CStateMachine* pSM;

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    CLONE(Ent_Monster);
    Ent_Monster();
    Ent_Monster(const Ent_Monster& _Other);
    ~Ent_Monster();

};

