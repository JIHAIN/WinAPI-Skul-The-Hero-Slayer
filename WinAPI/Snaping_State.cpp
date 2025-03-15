#include "pch.h"
#include "Snaping_State.h"

#include "Boss_Monster.h"
#include "CAnimator.h"
#include "BossLeft.h"
#include "BossRight.h"
#include "Boss_Body.h"
#include "Boss_Chin.h"

#include "CCamera.h"
#include "CSound.h"
#include "CAssetMgr.h"
#include "Effect_Obj.h"
#include "CLevel.h"
#include "CLevelMgr.h"

Snaping_State::Snaping_State()
    : Left_Hend_Atk(true)
    , Ani_One(true)
{
}

Snaping_State::~Snaping_State()
{
}


void Snaping_State::Enter()
{
    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;
    BossMonster->GetPart(Part);

    Vec2 Boss_Pos = BossMonster->GetPos();
    Vec2 Left_handPos = Part.Hand_Left->GetPos();
    Vec2 Right_handPos = Part.Hand_Right->GetPos();


    float groundLevel = 1550.f;  // 땅의 Y 좌표
    float HandGoal = 1000.f; // 손이 어디까지 갈지

    // 무슨 손이 공격 할거냐에 따라 방향을 정해줌 
    if (Left_handPos == Vec2(Boss_Pos.x - HandGoal, groundLevel)) // 왼쪽 손이 준비중이라면
    {
        Left_Hend_Atk = true;
    }
    else
    {
        Left_Hend_Atk = false;
    }

    CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"ElderEnt_Sweeping", L"sound\\ElderEnt_Sweeping.wav");
    m_Sound->Play();

    effect = new Effect_Obj;

    CCamera::Get()->SetCamShake(20.f, 5.f, 1.3f);
}

void Snaping_State::FinalTick()
{
    // 손은 보스 몬스터 x 좌표 중심 기준 600씩 이동할거임 즉 좌측 끝에서 우측으로 1200만큼 이동 위치는 보스 위치받아서 고정
    CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

    Boss_Monster* BossMonster = GetOwner<Boss_Monster>();
    Boss_Part Part;
    BossMonster->GetPart(Part);

    Vec2 Boss_Pos = BossMonster->GetPos();
    Vec2 Left_handPos = Part.Hand_Left->GetPos();
    Vec2 Right_handPos = Part.Hand_Right->GetPos();

 
 
    float groundLevel = 1550.f;  // 땅의 Y 좌표
    float HandGoal = 1000.f; // 손이 어디까지 갈지

    // 무슨 손이 공격 할거냐에 따라 방향을 정해줌 
    if (Left_Hend_Atk) // 왼쪽 손이 준비중이라면
    {


        if (Left_handPos.x < Boss_Pos.x + HandGoal)
        {
            Part.Hand_Left->SetPos(Left_handPos.x + 1500 * DT, groundLevel);
            Part.Hand_Left->LeftAtkOn(true);

            if (Ani_One)
            {
                Ani_One = false;

                CAnimator* EffAni = effect->GetEditAnimator();
                EffAni->Play(L"Snap_Effect_Right", true);
                effect->SetDuration(1.3f);
                effect->SetPos(Part.Hand_Left->GetPos());

                CurLevel->AddObject(effect, LAYER_TYPE::MONSTER_PROJECTILE);
            }
            Vec2 HandPos = Part.Hand_Left->GetPos();

            effect->SetPos(HandPos.x - 200.f, HandPos.y + 80.f);

        }
        else
        {
            GetStateMachine()->ChangeState(L"IdleState");
            Part.Hand_Left->LeftAtkOn(false);
        }
    }
    else
    {
        if (Right_handPos.x > Boss_Pos.x - HandGoal)
        {
            Part.Hand_Right->SetPos(Right_handPos.x - 1500 * DT, groundLevel);
            Part.Hand_Right->RightAtkOn(true);

            if (Ani_One)
            {
                Ani_One = false;

                CAnimator* EffAni = effect->GetEditAnimator();
                EffAni->Play(L"Snap_Effect_Left", true);
                effect->SetDuration(1.3f);
                effect->SetPos(Part.Hand_Right->GetPos());

                CurLevel->AddObject(effect, LAYER_TYPE::MONSTER_PROJECTILE);
            }

            Vec2 HandPos = Part.Hand_Right->GetPos();

            effect->SetPos(HandPos.x + 200.f, HandPos.y + 80.f);
        }
        else
        {
            GetStateMachine()->ChangeState(L"IdleState");
            Part.Hand_Right->RightAtkOn(false);
        }
    }

    


}

void Snaping_State::Exit()
{
    Ani_One = true;
}