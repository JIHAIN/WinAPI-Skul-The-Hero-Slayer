#include "pch.h"
#include "Ent_Atk_State.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"

#include "Ent_Atk.h"

void Ent_Atk_State::Enter()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	// 몬스터 공격 오브젝트 생성
	Ent_Atk* pNormal = new Ent_Atk;
	Vec2 AtkPos = pPlayer->GetPos();
	Vec2 AtkScale = pPlayer->GetScale();
	pNormal->SetName(L"Ent_Atk");
	pNormal->SetPos(AtkPos.x, AtkPos.y + AtkScale.y / 2 - 18.f);

	CreateObject(pNormal, LAYER_TYPE::MONSTER_PROJECTILE);

	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Ent_Atk_Ready", L"sound\\Ent_Atk_Ready.wav");
	pBGM->Play();

}

void Ent_Atk_State::FinalTick()
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
		MonAni->Play(L"Ent_Atk_Right", false);

	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Ent_Atk_Left", false);
	}



	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}

	


}

void Ent_Atk_State::Exit()
{
	
}

Ent_Atk_State::Ent_Atk_State()
{
}

Ent_Atk_State::~Ent_Atk_State()
{
}
