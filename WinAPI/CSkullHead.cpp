#include "pch.h"
#include "CSkullHead.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CTimeMgr.h"

CSkullHead::CSkullHead()
    : m_Speed(800.f)
    , m_MaxDistance(1000.f)
    , m_TraveledDistance(0.f)
    , m_IsGrounded(false)
    , m_IsFalling(false)
    , m_LastTeleportTime(0.f)
    , m_FallSpeed(300.f)
    , m_Animator(nullptr)
{
    m_Collider = AddComponent(new CCollider);
    m_Collider->SetScale(Vec2(40.f, 40.f));

    m_Animator = AddComponent(new CAnimator);
    m_Animator->LoadAnimation(L"animation\\Player_Attack\\Skul_Head_Left.anim");
    m_Animator->LoadAnimation(L"animation\\Player_Attack\\Skul_Head_Right.anim");

}

CSkullHead::~CSkullHead()
{
}

void CSkullHead::Tick()
{
    Vec2 curPos = GetPos();
    Vec2 newPos = curPos;

    if (!m_IsGrounded && !m_IsFalling)
    {
        newPos = curPos + m_Direction * m_Speed * DT;
        m_TraveledDistance += (newPos - curPos).Length();
        if (m_TraveledDistance >= m_MaxDistance)
        {
            m_IsFalling = true;
        }
    }
    else if (m_IsFalling)
    {
        newPos = curPos + Vec2(0.f, m_FallSpeed * DT);
    }

    SetPos(newPos);

    m_LastTeleportTime += DT;
    if (m_LastTeleportTime > 10.0f)
    {
        DeleteObject(this);
    }
}

void CSkullHead::Render()
{
    m_Animator->Render();
    // 렌더링 로직 (필요한 경우 구현)
}

void CSkullHead::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
    
    if (_OtherObj->GetName() == L"Monster" || _OtherObj->GetName() == L"Platform")
    {
        if (!m_IsGrounded && !m_IsFalling)
        {
            m_IsFalling = true;
        }
    }
    if (m_IsFalling && _OtherObj->GetName() == L"Floor" || _OtherObj->GetName() == L"Platform")
    {
        m_IsGrounded = true;
        m_IsFalling = false;
    }
}

void CSkullHead::Teleport(Vec2 newPosition)
{
    SetPos(newPosition);
    m_LastTeleportTime = 0.f;
}