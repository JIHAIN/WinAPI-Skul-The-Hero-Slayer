#include "pch.h"
#include "Mage_Attack_State.h"
#include "Mage_Atk.h"

#include "CTimeMgr.h"
#include "CMonster.h"
#include "CAnimator.h"


void Mage_Attack_State::Enter()
{
	// ���͸� �˾Ƴ���.
	CMonster* pMonster = GetOwner<CMonster>();
	// Player ��ġ ��������.
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	Vec2 MonPos = pMonster->GetPos();
	Vec2 MonScale = pMonster->GetScale();
	Vec2 PlePos = pPlayer->GetPos();

	Vec2 FireballPos(MonPos.x, MonPos.y - MonScale.y / 2 - 40.f); // �� �׳� ������ ������� 

	Vec2 vDir = PlePos - MonPos;
	vDir.Normalize();

	Fireball = new Mage_Atk;
	Fireball->SetName(L"Mage_Atk");
	Fireball->SetPos(FireballPos);

	Fireball->TargetDir(FireballPos, PlePos); // Ÿ���̶� �Ÿ����
	Fireball->TargetPos(PlePos);              // Ÿ����ġ

	CreateObject(Fireball, LAYER_TYPE::MONSTER_PROJECTILE);

	// ������ ���ؼ� �´� �ִϸ��̼� ���
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
