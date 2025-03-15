#include "pch.h"
#include "BossLeft.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CPlayer.h"
#include "CRigidBody.h"

BossLeft::BossLeft()
	:m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_TargetObj(nullptr)
	, m_LeftInitPos(0.f,0.f)
	, Left_Atk(false)
{
	// �ݶ��̴�
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(250.f, 250.f));
	m_Collider->SetOffset(Vec2(20.f, 20.f));

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Left_Idle.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Left_Snap.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Left_Stamp.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Left_Stamping.anim");

    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Left_Idle.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Left_Snap.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Left_Stamp.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Left_Stamping.anim");

	m_Animator->Play(L"Boss_Hand1_Left_Idle", true);
}

BossLeft::BossLeft(const BossLeft& _Other)
{

}

BossLeft::~BossLeft()
{
}


void BossLeft::Tick()
{
}

void BossLeft::Render_Part()
{
	m_Animator->Render();
}

void BossLeft::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
    CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
    if (pOtherObj && Left_Atk)
    {
        // �÷��̾� ������
        pOtherObj->HP_Lose(30);

        CRigidBody* pRigd = pOtherObj->GetRigidBody();

        // ������ ��ġ�� �÷��̾��� ��ġ�� �̿��Ͽ� �о�� ���� ���
        Vec2 pushDirection = pOtherObj->GetPos() - GetPos();
        pushDirection.Normalize();

        // �о�� ���� ũ�� ���� (�� ���� ������ �ʿ��� �� �ֽ��ϴ�)
        float pushForce = 5000.f;

        // �÷��̾ �о�� �� ����
        pRigd->AddForce(pushDirection * pushForce);

        // �÷��̾ ���߿� ���� �ʵ��� y�� �ӵ��� 0���� ���� (�ʿ��� ���)
        Vec2 currentVelocity = pRigd->GetVelocity();
        pRigd->SetVelocity(Vec2(currentVelocity.x, 0));

        // �÷��̾ ��� ���߿� ���� (������)
        pOtherObj->Player_Hit();

        
    }

}

void BossLeft::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
    CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
    if (pOtherObj && Left_Atk)
    {
        CRigidBody* pRigd = pOtherObj->GetRigidBody();
        pRigd->SetVelocity(Vec2(0.f, 0.f));
    }
    
}
