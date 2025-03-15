#pragma once
#include "CAttack.h"

class Sword_Atk_Obj :
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

    CLONE(Sword_Atk_Obj);
    Sword_Atk_Obj();
    Sword_Atk_Obj(const Sword_Atk_Obj& _Other);
    ~Sword_Atk_Obj();
};
