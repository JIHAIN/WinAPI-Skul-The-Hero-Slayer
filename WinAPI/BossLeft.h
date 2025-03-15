#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;

class BossLeft :
    public CObj
{
protected:
    CCollider* m_Collider;
    CAnimator* m_Animator;
    Vec2       m_LeftInitPos;

    bool Left_Atk;

    CObj* m_TargetObj;

public:
    // 몬스터 정보 채우기
    virtual CAnimator* GetMonsterAnimator() { return m_Animator; }

    void SetInitLeftPos(float _Posx, float _Posy) { m_LeftInitPos = Vec2(_Posx, _Posy); }
    Vec2 GetInitLeftPos() { return m_LeftInitPos; }

    void LeftAtkOn(bool _OnOff) { Left_Atk = _OnOff; }

    virtual void Tick() override;
    virtual void Render() override {};
    void Render_Part();
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(BossLeft);
    BossLeft();
    BossLeft(const BossLeft& _Other);
    ~BossLeft();

};
