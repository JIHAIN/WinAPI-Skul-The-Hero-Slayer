#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;

class Boss_Body :
    public CObj
{
protected:
    CCollider*       Left_Collider;
    CCollider*       Right_Collider;

    CAnimator*       m_Animator;
    tMonsterInfo     m_Info;         // 몬스터가 가져야할 스탯 구조체

    bool flor_On;

    CObj* m_TargetObj;

public:
    // 몬스터 정보 채우기
    virtual const tMonsterInfo& GetMonsterInfo() { return m_Info; }
    virtual CAnimator* GetMonsterAnimator() { return m_Animator; }
    virtual void  SetInitPos(Vec2 _Pos) { m_Info.InitPos = _Pos; }

    void Body_flor(bool _OnOff) { flor_On = _OnOff; }

    virtual void Tick() override;
    virtual void Render() override {};
    void Render_Part();
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(Boss_Body);
    Boss_Body();
    Boss_Body(const Boss_Body& _Other);
    ~Boss_Body();

};