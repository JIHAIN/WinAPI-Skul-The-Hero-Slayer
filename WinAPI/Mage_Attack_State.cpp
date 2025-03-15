#include "pch.h"
#include "Mage_Attack_State.h"
#include "Mage_Atk.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"


void Mage_Attack_State::Enter()
{
	// 몬스터를 알아낸다.
	CMonster* pMonster = GetOwner<CMonster>();
	// Player 위치 가져오기.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();

	Vec2 FireballPos(MonPos.x, MonPos.y - MonScale.y / 2 - 40.f); // 길어서 그냥 변수로 만들어줌 

	Vec2 vDir = PlePos - MonPos;
	vDir.Normalize();

	Fireball = new Mage_Atk;
	Fireball->SetName(L"Mage_Atk");
	Fireball->SetPos(FireballPos);

	Fireball->TargetDir(FireballPos, PlePos); // 타겟이랑 거리계산
	Fireball->TargetPos(PlePos);              // 타겟위치

	CreateObject(Fireball, LAYER_TYPE::MONSTER_PROJECTILE);

	// 방향을 구해서 맞는 애니메이션 재생
	if (vDir.x > 0)
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_ATK_Right", true);
	}
	else
	{
		CAnimator* MonAni = pMonster->GetMonsterAnimator();
		MonAni->Play(L"Mage_ATK_Left", true);
	}

	
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"Wizard_Charge_End", L"sound\\Wizard_Charge_End.wav");
	pBGM->Play();
	

}

void Mage_Attack_State::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	float AtkCoolTime = pMonster->GetMonsterInfo().AtkCoolTime;

	static float Cooltime = 0;
	Cooltime += DT;

	if (Cooltime >= AtkCoolTime)
	{
		Cooltime = 0;
		GetStateMachine()->ChangeState(L"IdleState");

	}




}

void Mage_Attack_State::Exit()
{
	
}

Mage_Attack_State::Mage_Attack_State()
	: Fireball(nullptr)
{
}

Mage_Attack_State::~Mage_Attack_State()
{
}
