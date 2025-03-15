#include "pch.h"
#include "BossRight.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

#include "CPlayer.h"

BossRight::BossRight()
	:m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_TargetObj(nullptr)
	, m_RightInitPos(0.f,0.f)
	, Right_Atk(false)
{
	// 콜라이더
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Monster_collider");
	m_Collider->SetScale(Vec2(250.f, 250.f));
	m_Collider->SetOffset(Vec2(-20.f, 20.f));


	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Right_Idle.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Right_Snap.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Right_Stamp.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand1_Right_Stamping.anim");

    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Right_Idle.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Right_Snap.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Right_Stamp.anim");
    m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Hand2_Right_Stamping.anim");

	m_Animator->Play(L"Boss_Hand1_Right_Idle", true);
}

BossRight::BossRight(const BossRight& _Other)
{

}

BossRight::~BossRight()
{
}


void BossRight::Tick()
{
}


void BossRight::Render_Part()
{
	m_Animator->Render();
}

void BossRight::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
    CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
    if (pOtherObj && Right_Atk)
    {
        // 플레이어 데미지
        pOtherObj->HP_Lose(30);

        CRigidBody* pRigd = pOtherObj->GetRigidBody();

        // 보스의 위치와 플레이어의 위치를 이용하여 밀어내는 방향 계산
        Vec2 pushDirection = pOtherObj->GetPos() - GetPos();
        pushDirection.Normalize();

        // 밀어내는 힘의 크기 설정 (이 값은 조정이 필요할 수 있습니다)
        float pushForce = 5000.f;

        // 플레이어를 밀어내는 힘 적용
        pRigd->AddForce(pushDirection * pushForce);

        // 플레이어가 공중에 뜨지 않도록 y축 속도를 0으로 설정 (필요한 경우)
        Vec2 currentVelocity = pRigd->GetVelocity();
        pRigd->SetVelocity(Vec2(currentVelocity.x, 0));

        // 플레이어를 잠시 공중에 띄우기 (선택적)
        pOtherObj->Player_Hit();


    }
}

void BossRight::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
    CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
    if (pOtherObj && Right_Atk)
    {
        CRigidBody* pRigd = pOtherObj->GetRigidBody();
        pRigd->SetVelocity(Vec2(0.f, 0.f));
    }
}
