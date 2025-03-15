#pragma once
#include "CAttack.h"

class Mage_Atk :
    public CAttack
{
private:
    Vec2     m_PlayerPos;
    Vec2     m_Dir;
    float    AccTime;
    bool     Deleteing;

public:
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

    void TargetDir(Vec2 _my, Vec2 _Taget);
    void TargetPos(Vec2 _TargetPos) { m_PlayerPos = _TargetPos;}

    CAnimator* GetAnimatorMageAtk() { return m_Animator; }

    CLONE(Mage_Atk);
    Mage_Atk();
    Mage_Atk(const Mage_Atk& _Other);
    ~Mage_Atk();
};

