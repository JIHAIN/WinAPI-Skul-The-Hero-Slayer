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
    
    if (LeftHand) // 왼쪽손이 더 가까우면
    {
        handPos = Part.Hand_Left->GetPos(); // 손위치를 왼쪽손으로 지정
    }
    else
    {
        handPos = Part.Hand_Right->GetPos(); // 아니라면 오른손으로 지정
    }
      

    Vec2 playerPos = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer()->GetPos(); // 플레이어 좌표

    float groundLevel = 1600.f;  // 땅의 Y 좌표

    // 시작 위치
    handPos;

    // 목표 위치 (플레이어 위치를 지나 땅까지)
    Vec2 targetPos = Vec2(playerPos.x, groundLevel);

    //6 이동 방향 벡터 계산
    Vec2 direction = targetPos - handPos;
    direction.Normalize();  // 방향 벡터를 정규화

    // 이동 속도
    float speed = 2000.f * DT;  // 예: 초당 300 단위로 이동

    // 새로운 위치 계산
    Vec2 newPos = handPos + direction * speed;    

    // 손의 위치 업데이트
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

    // 손이 땅에 닿았는지 확인
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