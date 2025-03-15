#include "pch.h"
#include "BEnt_Attack_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "BEnt_Atk.h"

void BEnt_Attack_State::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();

	// 몬스터 공격 오브젝트 생성
	BEnt_Atk* pNormal = new BEnt_Atk;
	Vec2 pPos = pMonster->GetPos();
	pNormal->SetName(L"BEnt_Atk");

	pNormal->SetPos(pPos);
	
	CreateObject(pNormal, LAYER_TYPE::MONSTER_PROJECTILE);

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Poison", L"sound\\Poison.wav");
	pBGM->Play();
}

void BEnt_Attack_State::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	float AtkCoolTime = pMonster->GetMonsterInfo().AtkCoolTime;

	Cooltime += DT;

	Vec2 MonPos = pMonster->GetPos();
	Vec2 vDir = pPlayer->GetPos() - MonPos;
	vDir.Normalize();

	if (vDir.x > 0)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"BEnt_ATK_Right", false);

	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"BEnt_ATK_Left", false);
	}



	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}




}

void BEnt_Attack_State::Exit()
{

}

BEnt_Attack_State::BEnt_Attack_State()
	: Cooltime(0.f)
{
}

BEnt_Attack_State::~BEnt_Attack_State()
{
}
