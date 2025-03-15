#pragma once
#include "CObj.h"

class CAnimator;
class CCollider;
class CPlayer;

class Dark_Back :
    public CObj
{
protected:
    CAnimator* m_Animator;
    CPlayer* m_Player;

    float      m_Duration;
    float      m_Acctime;

public:
    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(Dark_Back);
    Dark_Back();
    Dark_Back(const Dark_Back& _Other);
    ~Dark_Back();
};