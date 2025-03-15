#include "pch.h"
#include "Stamp_Enter_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "CSound.h"
#include "CAssetMgr.h"

Stamp_Enter_State::Stamp_Enter_State()
{
}

Stamp_Enter_State::~Stamp_Enter_State()
{
}


void Stamp_Enter_State::Enter()
{
	Boss_Monster* BossMonster = GetOwner<Boss_Monster>();

	Boss_Part Part;

	BossMonster->GetPart(Part);
	CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
	CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
	CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
	CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

	BodyAni->Play(L"Boss_Body1", true);
	LeftAni->Play(L"Boss_Hand1_Left_Stamp", true);
	RightAni->Play(L"Boss_Hand1_Right_Stamp", true);

	CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_Groggy_Recovery", L"sound\\ElderEnt_Groggy_Recovery.wav");
	m_Sound->Play();
}

void Stamp_Enter_State::FinalTick()
{
	static float AccTime = 0.f;    // ���� ��� ���ӽð�
	bool Stamping = false;  // �ִϸ��̼��� �����ߴ���.


	Boss_Monster* BossMonster = GetOwner<Boss_Monster>(); // ���� �ּ� �޾ƿ���

	Boss_Part Part; // ���� ������ ����ü

	BossMonster->GetPart(Part);

	Vec2 BossInitPos = BossMonster->GetMonsterInfo().InitPos; // ������ �� ������ �̴ϼ������ �� ������ ��ġ �޾ƿ���
	Vec2 bPos = BossMonster->GetPos();
	Vec2 LPos = Part.Hand_Left->GetPos();
	Vec2 RPos = Part.Hand_Right->GetPos();
	Vec2 CPos = Part.Chin->GetPos();

	float moveSpeed = 500 * DT;

	Vec2 Up = Vec2(0.f, -1.f) * moveSpeed;
	Vec2 Down = Vec2(0.f, 1.f) * moveSpeed;

	if (LPos.y > BossInitPos.y - 120.f)
	{
		Part.Hand_Left->SetPos(LPos + Up);
		Part.Hand_Right->SetPos(RPos + Up);
		AccTime = 0.f; 
		Stamping = false;
	}
	else
	{
		if (!Stamping)
		{
			CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
			CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

			LeftAni->Play(L"Boss_Hand1_Left_Stamping", true);
			RightAni->Play(L"Boss_Hand1_Right_Stamping", true);

			Stamping = true; // �ִϸ��̼� ����
		}
		AccTime += DT;
		
		if (AccTime >= 1.4f)
		{
			AccTime = 0.f; // �ð� �ʱ�ȭ
			Stamping = false;
			GetStateMachine()->ChangeState(L"Stamping");
		}
	}


}

void Stamp_Enter_State::Exit()
{

}