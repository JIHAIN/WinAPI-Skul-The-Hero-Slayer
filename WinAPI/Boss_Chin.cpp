#include "pch.h"
#include "Boss_Chin.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAnimator.h"
#include "CAssetMgr.h"

Boss_Chin::Boss_Chin()
	:m_Collider(nullptr)
	,m_RigidBody(nullptr)
	,m_Animator(nullptr)
	,m_Info{}
	,m_TargetObj(nullptr)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Chin1.anim");
	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Chin1_Row.anim");

	m_Animator->LoadAnimation(L"animation\\Boss\\Boss_Chin2.anim");
	

	m_Animator->Play(L"Boss_Chin1", true);
}

Boss_Chin::Boss_Chin(const Boss_Chin& _Other)
{

}

Boss_Chin::~Boss_Chin()
{
}


void Boss_Chin::Tick()
{
}

void Boss_Chin::Render_Part()
{
	m_Animator->Render();
}

void Boss_Chin::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}

