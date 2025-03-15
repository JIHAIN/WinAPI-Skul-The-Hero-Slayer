#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;

class CMonster :
    public CObj
{
protected:
    CCollider*   m_Collider;
    CRigidBody*  m_RigidBody;
    CAnimator*   m_Animator;

    int          m_HP;
    float        m_Speed;

    tMonsterInfo  m_Info; // 몬스터가 가져야할 스탯 구조체

    CObj*       m_TargetObj;
    float       m_DetectRange;

    bool Wall;
    Vec2 m_PrevPos;

public:
    void SetWall(bool _true) { Wall = _true; }
    bool GetWall() { return Wall; }
    Vec2 GetPrevPos() { return m_PrevPos; }

    // 몬스터 정보 채우기
    virtual const tMonsterInfo& GetMonsterInfo() { return m_Info; }
    virtual CAnimator*  GetMonsterAnimator() { return m_Animator; }
    virtual void  SetInitPos(Vec2 _Pos) { m_Info.InitPos = _Pos; }
    virtual CRigidBody* GetRigid() { return m_RigidBody; }
    virtual void SetScale(float _Width, float _Height) override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CMonster);
    CMonster();
    CMonster(const CMonster& _Other);
    ~CMonster();
};

