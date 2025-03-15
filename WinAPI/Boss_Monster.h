#pragma once
#include "CObj.h"

class CCollider;
class CRigidBody;
class CAnimator;
class Boss_Body;
class Boss_Chin;
class BossLeft;
class BossRight;

struct Boss_Part
{
    BossLeft*  Hand_Left;
    BossRight* Hand_Right;
    Boss_Chin* Chin;
    Boss_Body* Body;
};

class Boss_Monster :
    public CObj
{
protected:
    CCollider*   m_Collider;
    CRigidBody*  m_RigidBody;

    CAnimator*   m_Animator;

    BossLeft*    Hand_Left;
    BossRight*   Hand_Right;
    Boss_Chin*   Chin;
    Boss_Body*   Body;

    bool    Boss_Atk_Cheak;
    bool    Boss_flor_Cheak;
    bool    phase2;


    tMonsterInfo  m_Info; // ���Ͱ� �������� ���� ����ü

    CObj*       m_TargetObj;


public:
    // ���� ���� ä���
    virtual const tMonsterInfo& GetMonsterInfo() { return m_Info; }

    virtual void SetInitPos(Vec2 _Pos) { m_Info.InitPos = _Pos; }
    virtual void SetScale(float _Width, float _Height) override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;

    void Left_Atk_On(bool _Atk);
    void Right_Atk_On(bool _Atk);

    void flor_On(bool _flor);

    // ���� �޾ƿ���
    BossLeft* GetLeftHand() { return Hand_Left; }
    BossRight* GetRightHand() { return Hand_Right; }
    Boss_Chin* GetChin() { return Chin; }
    Boss_Body* GetBody() { return Body; }
    CAnimator* GetMonsterAnimator() { return m_Animator; }

    void GetPart(Boss_Part& _Part);


public:
    CLONE(Boss_Monster);
    Boss_Monster();
    Boss_Monster(Vec2 _Pos); // ��ġ�޴� ����
    Boss_Monster(const Boss_Monster& _Other);
    ~Boss_Monster();
};
