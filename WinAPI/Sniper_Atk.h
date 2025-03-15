#pragma once
#include "CAttack.h"

class Sniper_Atk :
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

    CAnimator* GetAnimatorMageAtk() { return m_Animator; }

    CLONE(Sniper_Atk);
    Sniper_Atk(bool _Left);
    Sniper_Atk(const Sniper_Atk& _Other);
    ~Sniper_Atk();
};