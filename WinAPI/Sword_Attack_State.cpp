#include "pch.h"
#include "Sword_Attack_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "Sword_Atk_Obj.h"

void Sword_Attack_State::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 vDir = pPlayer->GetPos() - MonPos;
	vDir.Normalize();

	// 몬스터 공격 오브젝트 생성
	Sword_Atk_Obj* pNormal = new Sword_Atk_Obj;
	pNormal->SetName(L"Sword_Atk");
	if (vDir.x < 0.f) // 왼쪽에 있을시
	{
		pNormal->SetPos(MonPos.x - 40.f, MonPos.y);
	}
	else
	{
		pNormal->SetPos(MonPos.x + 40.f, MonPos.y);
	}

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Atk_Sword_Small", L"sound\\Atk_Sword_Small.wav");
	pBGM->Play();

	CreateObject(pNormal, LAYER_TYPE::MONSTER_PROJECTILE);

}

void Sword_Attack_State::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	float AtkCoolTime = pMonster->GetMonsterInfo().AtkCoolTime;

	static float Cooltime = 0;
	Cooltime += DT;

	Vec2 MonPos = pMonster->GetPos();
	Vec2 vDir = pPlayer->GetPos() - MonPos;
	vDir.Normalize();

	if (vDir.x > 0)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Sword_Atk_Right", false);

	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Sword_Atk_Left", false);
	}



	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}




}

void Sword_Attack_State::Exit()
{

}

Sword_Attack_State::Sword_Attack_State()
{
}

Sword_Attack_State::~Sword_Attack_State()
{
}

