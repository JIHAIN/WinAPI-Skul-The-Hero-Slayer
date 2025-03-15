#pragma once
#include "CObj.h"

class CTexture;
class CCollider;
class CAnimator;
class CRigidBody;
class CSkullHead;
class CStateMachine;
class CSound;

class CPlayer :
    public CObj
{
private:
    CTexture* m_Texture;
    CSound* m_Sound;

    float       m_Speed;
    int         m_Player_HP;
    int         m_Player_MaxHp;
    bool        m_Darkporm;

    bool        m_lastRightPressed; // ���������� ������ Ű�� �������� ���θ� ����

    int         m_AtkCount;
    float       m_AttackCooldown;
    bool        m_Floor;
    int         m_DoubleJump;
    Vec2        m_PrevPos;

    int         m_DashCount;        // ���� ��� Ƚ��
    float       m_DashCooldown;     // ��� ��ٿ�
    float       m_DashDistance;     // �� ��� �Ÿ�
    float       m_DashSpeed;        // ��� �ӵ�
    Vec2        m_DashDirection;    // ��� ����
    bool        m_IsDashing;        // ���� ��� ������ ����
    float       m_DashProgress;     // ���� ��� ���� �Ÿ�
    bool        m_CanSecondDash;    // 2�� �뽬 ���� ����
    float       m_DashInputWindow;  // �� ��° ��� �Է� ���� �ð�
    float       m_DashInputTimer;   // ù ��° ��� �� ��� �ð�

    CSkullHead* m_ThrownHead;       // �Ӹ������� ��ų
    bool        m_IsHeadless;       // ���ɿ���
    Vec2        m_TeleportPosition; // �ڷ���Ʈ ��ġ
    bool        m_ShouldTeleport;   //�ڷ���Ʈ ��������

    CCollider* m_Body;
    CAnimator* m_Animator;
    CRigidBody* m_RigidBody;
    CStateMachine* PlayerState;

public:
    virtual void Tick() override;
    virtual void Render() override;

    bool GetPlayerprom() { return m_Darkporm; }
    void Attack(bool _LastRight);
    void Dark_Atk(bool _LastRight);
    void DownOn(bool _truefalse) { m_Floor = _truefalse; }

    int GetHP() { return m_Player_HP; }
    int GetMaxHP() { return m_Player_MaxHp; }

    CAnimator* GetPlayerAnimator() { return m_Animator; }
    bool GetHeadless() { return m_IsHeadless; }
    bool GetRightPrd() { return m_lastRightPressed; }
    bool GetGround();
    void SetAttackCount(int _Count) { m_AtkCount = _Count; }

    void StartDash();  //�뽬����
    void UpdateDash();


    void ThrowHead();   //�Ӹ� ������ ����
    void TeleportToHead();
    void DarkWave();

    // �浹 ó���� �÷��̾�� ������ �� �ڵ�
    void HP_Lose(int _Damage) { m_Player_HP -= _Damage; }
    CRigidBody* GetRigidBody() { return m_RigidBody; }
    void Player_Hit();

public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
public:
    CLONE(CPlayer);
    CPlayer();
    CPlayer(const CPlayer& _Other);
    ~CPlayer();
};

