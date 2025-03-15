#include "pch.h"
#include "Balling_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"
#include "Boss_Ball_Create.h"
#include "CTimeMgr.h"

#include "Effect_Obj.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CCamera.h"
#include "Boss_Effect.h"

#include "CAssetMgr.h"
#include "CSound.h"

Balling_State::Balling_State()
    : BallAtk(false)
    , AccTime(0.f)
{
}

Balling_State::~Balling_State()
{
}


void Balling_State::Enter()
{
    CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;

    BossMonster->GetPart(Part);
    CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
    CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
    CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
    CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

   // BodyAni->Play(L"Boss_Body1", true);
   // LeftAni->Play(L"Boss_Hand1_Left_Stamp", true);
   // RightAni->Play(L"Boss_Hand1_Right_Stamp", true);

    Vec2 bPos = BossMonster->GetPos();

    BossMonster->GetPart(Part);
   

    // º¼ »ý¼º 
    Boss_Ball_Create* Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x - 200.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x - 200.f, bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

    Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x - 400.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x - 400.f, bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

    Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x - 600.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x - 600.f, bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

    Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x + 200.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x + 200.f, bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

    Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x + 400.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x + 400.f, bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

    Boss_Ball = new Boss_Ball_Create(Vec2(bPos.x + 600.f, bPos.y - 200.f));
    Boss_Ball->SetPos(Vec2(bPos.x + 600.f , bPos.y - 200.f));
    CurLevel->AddObject(Boss_Ball, LAYER_TYPE::MONSTER_PROJECTILE);

  
}

void Balling_State::FinalTick()
{
   
   AccTime += DT;

   if (AccTime > 2.5f)
   {
       AccTime = 0;

       CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_EnergyBomb_Fire", L"sound\\ElderEnt_EnergyBomb_Fire.wav");
       m_Sound->Play();

       GetStateMachine()->ChangeState(L"IdleState");
   }

}

void Balling_State::Exit()
{
    
}