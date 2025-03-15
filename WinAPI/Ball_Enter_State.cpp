#include "pch.h"
#include "Ball_Enter_State.h"

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
#include "Boss_Effect.h"
#include "CTimeMgr.h"

#include "CSound.h"
#include "CAssetMgr.h"

Ball_Enter_State::Ball_Enter_State()
    : BallAtk(false)
    , AccTime(0.f)
{
}

Ball_Enter_State::~Ball_Enter_State()
{
}


void Ball_Enter_State::Enter()
{
    CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;
    
    CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_EnergyBomb_Ready", L"sound\\ElderEnt_EnergyBomb_Ready.wav");
    m_Sound->Play();

    BossMonster->GetPart(Part);
    CAnimator* BodyAni = Part.Body->GetMonsterAnimator();
    CAnimator* ChinAni = Part.Chin->GetMonsterAnimator();
    CAnimator* LeftAni = Part.Hand_Left->GetMonsterAnimator();
    CAnimator* RightAni = Part.Hand_Right->GetMonsterAnimator();

    BodyAni->Play(L"Boss_Body1", true);
    LeftAni->Play(L"Boss_Hand1_Left_Stamp", true);
    RightAni->Play(L"Boss_Hand1_Right_Stamp", true);

    Vec2 bPos = BossMonster->GetPos();
    Boss_Effect* effect = new Boss_Effect;
    CAnimator* EftAni = effect->GetEditAnimator();
    EftAni->Play(L"Boss_Ball_Enter", false);

    effect->SetPos(bPos.x, bPos.y - 20.f);
    CurLevel->AddObject(effect, LAYER_TYPE::MONSTER_PROJECTILE);
}

void Ball_Enter_State::FinalTick()
{
    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;
    BossMonster->GetPart(Part);
    AccTime += DT;

    if (AccTime > 2.4f)
    {
        AccTime = 0;
        GetStateMachine()->ChangeState(L"Balling_State");
    }
   
}

void Ball_Enter_State::Exit()
{
}