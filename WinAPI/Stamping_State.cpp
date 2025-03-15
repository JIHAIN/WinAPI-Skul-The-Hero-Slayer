#include "pch.h"
#include "Stamping_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "Effect_Obj.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CSound.h"

Stamping_State::Stamping_State()
    : LeftHand(false)
{
}

Stamping_State::~Stamping_State()
{
}


void Stamping_State::Enter()
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

    Vec2 bPos = BossMonster->GetPos();

    CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

    Vec2 pPos = pPlayer->GetPos();

    if (bPos.x > pPos.x)
    {
        LeftHand = true;
    }
    else
    {
        LeftHand = false;
    }

    CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_FistSlam_Roar", L"sound\\ElderEnt_FistSlam_Roar.wav");
    m_Sound->Play();

}

void Stamping_State::FinalTick()
{
    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;
    BossMonster->GetPart(Part);

    Vec2 handPos;
    
    if (LeftHand) // ���ʼ��� �� ������
    {
        handPos = Part.Hand_Left->GetPos(); // ����ġ�� ���ʼ����� ����
    }
    else
    {
        handPos = Part.Hand_Right->GetPos(); // �ƴ϶�� ���������� ����
    }
      

    Vec2 playerPos = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer()->GetPos(); // �÷��̾� ��ǥ

    float groundLevel = 1600.f;  // ���� Y ��ǥ

    // ���� ��ġ
    handPos;

    // ��ǥ ��ġ (�÷��̾� ��ġ�� ���� ������)
    Vec2 targetPos = Vec2(playerPos.x, groundLevel);

    //6 �̵� ���� ���� ���
    Vec2 direction = targetPos - handPos;
    direction.Normalize();  // ���� ���͸� ����ȭ

    // �̵� �ӵ�
    float speed = 2000.f * DT;  // ��: �ʴ� 300 ������ �̵�

    // ���ο� ��ġ ���
    Vec2 newPos = handPos + direction * speed;    

    // ���� ��ġ ������Ʈ
    if (LeftHand)
    {
        Part.Hand_Left->SetPos(newPos);
        Part.Hand_Left->LeftAtkOn(true);
    }
    else
    {
        Part.Hand_Right->SetPos(newPos);
        Part.Hand_Right->RightAtkOn(true);
    }

    // ���� ���� ��Ҵ��� Ȯ��
    if (newPos.y >= groundLevel - 70.f)
    {
        CCamera::Get()->SetCamShake(20.f, 5.f, 1.3f);

        newPos.y = groundLevel;
        newPos.x = handPos.x;

        CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
          
        CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_FistSlam", L"sound\\ElderEnt_FistSlam.wav");
        m_Sound->Play();

        Effect_Obj* effect = new Effect_Obj;
        effect->SetPos(newPos.x, newPos.y - 150.f);
        effect->SetDuration(1.2f);

        CAnimator* EffAni = effect->GetEditAnimator();
        EffAni->Play(L"Boss_Stamp_Effect_Left", false);

        CurLevel->AddObject(effect, LAYER_TYPE::MONSTER_PROJECTILE);

        Part.Hand_Right->RightAtkOn(false);
        Part.Hand_Left->LeftAtkOn(false);


        GetStateMachine()->ChangeState(L"Stamp_End");
    }


}

void Stamping_State::Exit()
{

}