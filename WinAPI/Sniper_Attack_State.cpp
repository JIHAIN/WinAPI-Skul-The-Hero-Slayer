#include "pch.h"
#include "Sniper_Attack_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"

#include "Sniper_Atk.h"



void Sniper_Attack_State::Enter()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CMonster* pMonster = GetOwner<CMonster>();

	float vDir;

	bool Left = false;

	if (pPlayer->GetPos().x < pMonster->GetPos().x)
	{
		vDir = -20.f;
		Left = true;
	}
	else
	{
		vDir = 20.f;
		Left = false;
	}

	

	// 몬스터 공격 오브젝트 생성
	Sniper_Atk* pNormal = new Sniper_Atk(Left);
	Vec2 AtkPos = pMonster->GetPos();
	pNormal->SetName(L"Sniper_Atk");
	pNormal->SetPos(AtkPos.x + vDir, AtkPos.y);

	pNormal->TargetDir(Vec2(AtkPos.x + vDir, AtkPos.y), pPlayer->GetPos());

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Hunter_Attack2", L"sound\\Hunter_Attack2.wav");
	pBGM->Play();

	CreateObject(pNormal, LAYER_TYPE::MONSTER_PROJECTILE);

}

void Sniper_Attack_State::FinalTick()
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
		MonAni->Play(L"Sniper_aiming_Right", false);

	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Sniper_aiming_Left", false);
	}



	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}




}

void Sniper_Attack_State::Exit()
{

}

Sniper_Attack_State::Sniper_Attack_State()
{
}

Sniper_Attack_State::~Sniper_Attack_State()
{
}
