#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;


class Boss_Chin :
    public CObj
{
protected:
     CCollider*     m_Collider;
     CRigidBody*    m_RigidBody;
     CAnimator*     m_Animator;
     tMonsterInfo   m_Info;         // ���Ͱ� �������� ���� ����ü

     CObj*          m_TargetObj;

public:
     // ���� ���� ä���
     virtual const tMonsterInfo& GetMonsterInfo() { return m_Info; }
     virtual CAnimator* GetMonsterAnimator() { return m_Animator; }
     virtual void  SetInitPos(Vec2 _Pos) { m_Info.InitPos = _Pos; }

     virtual void Tick() override;
     virtual void Render() override {};
     void Render_Part();
     virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
     CLONE(Boss_Chin);
     Boss_Chin();
     Boss_Chin(const Boss_Chin& _Other);
     ~Boss_Chin();
 
};

