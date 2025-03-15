#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;

class BossRight :
    public CObj
{
protected:
    CCollider*  m_Collider;
    CAnimator*  m_Animator;

    Vec2    m_RightInitPos;

    bool   Right_Atk;

    CObj* m_TargetObj;

public:
    // 몬스터 정보 채우기
    virtual CAnimator* GetMonsterAnimator() { return m_Animator; }

    void SetInitRightPos(float _Posx, float _Posy) { m_RightInitPos = Vec2(_Posx , _Posy) ; }
    Vec2 GetInitRightPos() {return m_RightInitPos;}

    void RightAtkOn(bool _OnOff) { Right_Atk = _OnOff; }

    virtual void Tick() override;
    virtual void Render() override {};
    void Render_Part();
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(BossRight);
    BossRight();
    BossRight(const BossRight& _Other);
    ~BossRight();

};