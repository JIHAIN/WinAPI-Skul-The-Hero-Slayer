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

    bool        m_lastRightPressed; // 마지막으로 오른쪽 키를 눌렀는지 여부를 추적

    int         m_AtkCount;
    float       m_AttackCooldown;
    bool        m_Floor;
    int         m_DoubleJump;
    Vec2        m_PrevPos;

    int         m_DashCount;        // 현재 대시 횟수
    float       m_DashCooldown;     // 대시 쿨다운
    float       m_DashDistance;     // 총 대시 거리
    float       m_DashSpeed;        // 대시 속도
    Vec2        m_DashDirection;    // 대시 방향
    bool        m_IsDashing;        // 현재 대시 중인지 여부
    float       m_DashProgress;     // 현재 대시 진행 거리
    bool        m_CanSecondDash;    // 2단 대쉬 실행 여부
    float       m_DashInputWindow;  // 두 번째 대시 입력 가능 시간
    float       m_DashInputTimer;   // 첫 번째 대시 후 경과 시간

    CSkullHead* m_ThrownHead;       // 머리던지기 스킬
    bool        m_IsHeadless;       // 가능여부
    Vec2        m_TeleportPosition; // 텔레포트 위치
    bool        m_ShouldTeleport;   //텔레포트 가능한지

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

    void StartDash();  //대쉬관련
    void UpdateDash();


    void ThrowHead();   //머리 던지기 관련
    void TeleportToHead();
    void DarkWave();

    // 충돌 처리시 플레이어에게 영향을 줄 코드
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

