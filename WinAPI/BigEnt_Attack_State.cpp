#include "pch.h"
#include "BigEnt_Attack_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "BigEnt_Atk.h"


void BigEnt_Attack_State::Enter()
{
	// Trace 상태의 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();

	CAnimator* MonAni = pMonster->GetMonsterAnimator();
	MonAni->Play(L"BigEnt_Atk", false);

	fistatk = 0;

	

}

void BigEnt_Attack_State::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	

	float AtkCoolTime = pMonster->GetMonsterInfo().AtkCoolTime;

	static float Cooltime = 0;
	Cooltime += DT;

	if (Cooltime >= 0.6f && fistatk == 0)
	{
		fistatk += 1;

		// 몬스터 공격 오브젝트 생성
		BigEnt_Atk* pNormal = new BigEnt_Atk;
		Vec2 pPos = pMonster->GetPos();
		Vec2 pScale = pMonster->GetScale();
		pNormal->SetName(L"BigEnt_Atk");

		pNormal->SetPos(pPos.x, pPos.y + 50.f);

		CSound* pBGM = CAssetMgr::Get()->LoadSound(L"GiganticEnt_RangeAttack", L"sound\\GiganticEnt_RangeAttack.wav");
		pBGM->Play();

		CreateObject(pNormal, LAYER_TYPE::MONSTER_PROJECTILE);
	}



	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}




}

void BigEnt_Attack_State::Exit()
{

}

BigEnt_Attack_State::BigEnt_Attack_State()
{
}

BigEnt_Attack_State::~BigEnt_Attack_State()
{
}

