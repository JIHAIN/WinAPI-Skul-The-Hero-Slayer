#include "pch.h"
#include "CPlayer.h"
#include "CEngine.h"

//매니저
#include "CDebugMgr.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CKeyMgr.h"
#include "CTaskMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CCamera.h"
#include "CLogMgr.h"

#include "CLevel.h"

//랜더 관련
#include "CTexture.h"
#include "CAnimator.h"
#include "CAnim.h"

//컴포넌트
#include "CCollider.h"
#include "CRigidBody.h"
#include "CStateMachine.h"

//상태머신
#include "Attack1_State.h"
#include "Attack2_State.h"
#include "Attack3_State.h"
#include "JumpAtk_State.h"
#include "Left_State.h"
#include "Right_State.h"
#include "Jump_State.h"
#include "Upfall_State.h"
#include "Fall_State.h"
#include "Dash_State.h"
#include "LeftIdle_State.h"
#include "RightIdle_State.h"
#include "Dark_Wave_State.h"

//오브젝트
#include "CMissile.h"
#include "CGuidedMissile.h"
#include "CAttack.h"
#include "CKiMissile.h"
#include "CSkullHead.h"
#include "Dark_Attack.h"
#include "CPlayer_Normal_Attack.h"

#include "Player_State_UI.h"
#include "Dark_Back.h"

// 이펙트
#include "Dash_Effect.h"
#include "Jump_Effect.h"
#include "Dark_Waves.h"

// 사운드
#include "CSound.h"


CPlayer::CPlayer()
	: m_Speed(500.f)
	, m_Texture(nullptr)
	, m_Body(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Floor(false)
	, m_lastRightPressed(true)
	, m_AttackCooldown(2.f)
	, m_AtkCount(1)
	, m_DashCount(0)
	, m_DashCooldown(0.f)
	, m_DashDistance(300.f) // 총 대시 거리
	, m_DashSpeed(2000.f)   // 대시 속도
	, m_IsDashing(false)
	, m_DashProgress(0.f)
	, m_CanSecondDash(false)
	, m_ShouldTeleport(false)
	, m_ThrownHead(nullptr)
	, m_IsHeadless(false)
	, m_DashInputWindow(0.3f)
	, m_DashInputTimer(0.f)
	, m_DoubleJump(2)
	, m_PrevPos(0.f, 0.f)
	, m_Player_HP(200)
	, m_Player_MaxHp(200)
	, m_Darkporm(false)
	, m_Sound(nullptr)
{
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();




	//m_Texture = CAssetMgr::Get()->LoadTexture(L"Fighter", L"texture\\Fighter.bmp");

	//if (m_Texture)
	//{
	//	SetScale((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
	//}

	// 컴포넌트 설정
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"Player");

	m_Body->SetOffset(Vec2(0.f, 10.f));
	m_Body->SetScale(Vec2(50.f, 80.f));

	// RigidBody
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(1000.f);
	m_RigidBody->SetFriction(100.f);
	m_RigidBody->SetMaxGravitySpeed(1200.f);
	m_RigidBody->SetJumpSpeed(1.5f);


	// Player 에 Animator 컴포넌트 추가
	m_Animator = AddComponent(new CAnimator);

	//애니메이션 참고용
	{
		//tAnimDesc desc = {};
	//desc.pAtlas = CAssetMgr::Get()->LoadTexture(L"Skul", L"texture\\Skul_1.bmp");
	//desc.AnimName = L"IDLE_Right";
	//desc.FPS = 1;
	//desc.FrmCount = 1;
	//desc.SliceSize = Vec2(120.f, 130.f);
	//desc.StartLeftTop = Vec2(480.f, 0.f);	
	//m_Animator->CreateAnimation(desc);
	//내가 만든 코드 프레임 추가 사용법 
	//tAnimFrm desd = {};
	//desd.Duration = 1.f;
	//desd.vLeftTop = Vec2(0.f, 0.f);
	//desd.vOffset = Vec2(0.f, 0.f);
	//desd.vSlice = Vec2(120.f, 130.f);
	//m_Animator->AddFrameToAnimation(L"IDLE_Right" ,desd);
	//desc.AnimName = L"MOVE_Left";
	//desc.StartLeftTop = Vec2(0.f, 130.f);
	//desc.FrmCount = 8;
	//desc.SliceSize = Vec2(130.f, 130.f);
	//desc.FPS = 1;
	//m_Animator->CreateAnimation(desc);
	// 특정 프레임 찾아서 수정 방법
	//CAnim* pIdleRight = m_Animator->FindAnimation(L"IDLE_RIGHT");
	//pIdleRight->GetFrame(1).vOffset = Vec2(1.f, 0.f);
	// Animation 정보 저장하기 
	//m_Animator->SaveAnimation(L"animation\\Player\\");
	}

	//상태머신 추가
	PlayerState = AddComponent(new CStateMachine);

	PlayerState->AddState(L"Attack1", new Attack1_State);
	PlayerState->AddState(L"Attack2", new Attack2_State);
	PlayerState->AddState(L"Attack3", new Attack3_State);
	PlayerState->AddState(L"JumpAtk", new JumpAtk_State);
	PlayerState->AddState(L"Dash", new Dash_State);
	PlayerState->AddState(L"Fall", new Fall_State);
	PlayerState->AddState(L"Jump", new Jump_State);
	PlayerState->AddState(L"Upfall", new Upfall_State);
	PlayerState->AddState(L"Left", new Left_State);
	PlayerState->AddState(L"Right", new Right_State);
	PlayerState->AddState(L"LeftIdle", new LeftIdle_State);
	PlayerState->AddState(L"RightIdle", new RightIdle_State);
	PlayerState->AddState(L"Dark_Wave", new Dark_Wave_State);




	PlayerState->ChangeState(L"RightIdle");


	// 정보 로드

	/////////////////////// 왼쪽////////////////////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\IDLE_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\MOVE_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\UP_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\UP_fall_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\falling_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dash_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\ATTACK1_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\ATTACK2_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Jump_Atk_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Skul_Skil_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Skul_Change_Left.anim");

	//////////////////// 왼쪽 머리 없는 버전///////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Idle_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Move_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Dash_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Head_Up_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Fall_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_FallDown_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Head_Attack1_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Attack2_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_JumpAtk_Left.anim");



	///////////////// 오른쪽 ///////////////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\IDLE_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\MOVE_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dash_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\UP.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\UP_fall.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\falling.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\ATTACK1.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\ATTACK2.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Jump_Atk_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Skul_Skil_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Skul_Change_Right.anim");

	/////////////// 오른쪽 머리 없는 버전//////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Idle_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Move_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Dash_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Head_Up_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Fall_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_FallDown_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Head_Attack1_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_Attack2_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Head_JumpAtk_Right.anim");


	//////////////////다크 팔라딘 모드///////////////////////////
	/////////////////////// 왼쪽////////////////////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackA_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackB_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackC_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Dash_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Fall_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_FallRepeat_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Idle_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Jump_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Wave_Left.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Move_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Ready_Left.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_SkillA_Left.anim");

	///////////////// 오른쪽 ///////////////////////////////
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackA_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackB_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_AttackC_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Dash_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Fall_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_FallRepeat_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Idle_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Jump_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Effect\\Dark_Wave_Right.anim");

	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Move_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_Ready_Right.anim");
	m_Animator->LoadAnimation(L"animation\\Player\\Dark_SkillA_Right.anim");


	m_Animator->SetNextAnimation(L"ATTACK1", L"ATTACK2");

	// Animation 플레이
	//m_Animator->Play(L"IDLE_Right", true);

}

CPlayer::CPlayer(const CPlayer& _Other)
	: CObj(_Other) // 부모껀 부모가 알아서
	, m_Body(nullptr)
	, m_RigidBody(nullptr)
	, m_Animator(nullptr)
	, m_Floor(false)
	, m_Speed(_Other.m_Speed)
	, m_Texture(nullptr)
	, m_AttackCooldown(_Other.m_AttackCooldown)
	, m_DashCooldown(_Other.m_DashCooldown)
	, m_lastRightPressed(false)
{
	// 컴포넌트를 다시 겟 컴포넌트를 해주고
	m_Animator = GetComponent<CAnimator>();
	m_RigidBody = GetComponent<CRigidBody>();

	//콜라이더는 내 설정대로 하기 위해 오브젝트에 이름으로 콜라이더를 찾아오는 함수를 만들어서
	//그걸로 찾아온다
	m_Body = (CCollider*)GetComponentByName(L"Player");
}

CPlayer::~CPlayer()
{

}

void CPlayer::Tick()
{
	Vec2 vPos = GetPos();
	static int DashCount = 0;
	static float doudle = 1.f;
	static float DashY = 0;
	bool DDash = false;

	m_AttackCooldown += DT;

	if (!m_Darkporm)
	{
		if (m_PrevPos.y < vPos.y) // 현 위치가 전 위치보다 아래에 있을때
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Jump");
			}
			PlayerState->ChangeState(L"Fall");
		}

		else if (m_PrevPos.y > vPos.y) // 현 위치가 전 위치보다 위에 있을때
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Jump");
			}
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			m_lastRightPressed = false;

			vPos.x -= 500.f * DT;

			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Left");
			}
		}

		if (KEY_RELEASED(KEY::LEFT))
		{
			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"LeftIdle");
			}
		}


		if (KEY_PRESSED(KEY::RIGHT))
		{
			m_lastRightPressed = true;
			vPos.x += 500.f * DT;

			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Right");
			}
		}
		if (KEY_RELEASED(KEY::RIGHT))
		{
			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"RightIdle");
			}
		}

		if (KEY_PRESSED(KEY::UP))
		{
			m_RigidBody->SetGround(true);
			vPos.y -= 200.f * DT;

			if (m_lastRightPressed) // 마지막이 오른쪽이였다면
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"UP", true);
				}
				else
				{
					m_Animator->Play(L"Head_Up_Right", true);
				}
			}
			else
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"UP_Left", true);
				}
				else
				{
					m_Animator->Play(L"Head_Up_Left", true);
				}
			}


		}
		if (KEY_RELEASED(KEY::UP))
		{
			if (m_lastRightPressed) // 마지막이 오른쪽이였다면
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"falling", true);
				}
				else
				{
					m_Animator->Play(L"Head_Fall_Right", true);
				}
			}
			else
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"falling_Left", true);
				}
				else
				{
					m_Animator->Play(L"Head_Fall_Left", true);
				}
			}

		}


		if (KEY_TAP(KEY::DOWN))
		{
			DownOn(true);
			vPos.y += 100.f;
		}

		// 대쉬관련 구현
		if (KEY_TAP(KEY::Z))
		{

			StartDash();
		}

		UpdateDash();

		// 대시 중일 때 이동 적용
		if (m_IsDashing)
		{
			if (m_lastRightPressed) // 오른쪽 눌렸으면
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"Dash_Right", true);
				}
				else
				{
					m_Animator->Play(L"Head_Dash_Right", true);
				}
			}
			else
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"Dash_Left", true);
				}
				else
				{
					m_Animator->Play(L"Head_Dash_Left", true);
				}
			}
			vPos += m_DashDirection * (m_DashSpeed * DT);
		}


		// 점프관련 구현
		if (KEY_TAP(KEY::C))
		{
			if (m_DoubleJump == 2)
			{
				SetPos(vPos.x, vPos.y -= 2.f);
				m_RigidBody->Jump();
				--m_DoubleJump;

				Jump_Effect* jump_ef = new Jump_Effect;
				jump_ef->SetPos(vPos.x, vPos.y + 50.f);

				m_Sound = CAssetMgr::Get()->LoadSound(L"Jump", L"sound\\Default_Jump.wav");
				m_Sound->Play();

				CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
				CurLevel->AddObject(jump_ef, LAYER_TYPE::PLAYER_PROJECTILE);
			}
			else if (m_DoubleJump == 1)
			{
				SetPos(vPos.x, vPos.y -= 2.f);
				m_RigidBody->Jump();
				--m_DoubleJump;

				Jump_Effect* jump_ef = new Jump_Effect;
				jump_ef->SetPos(vPos.x, vPos.y + 50.f);

				m_Sound = CAssetMgr::Get()->LoadSound(L"AirJump", L"sound\\Default_Jump_Air.wav");
				m_Sound->Play();

				CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
				CurLevel->AddObject(jump_ef, LAYER_TYPE::PLAYER_PROJECTILE);
			}

		}

		if (m_RigidBody->IsGround())
		{
			m_DoubleJump = 2;
		}


		// 처음 공격
		if (KEY_TAP(KEY::X) && m_AtkCount == 1 && m_AttackCooldown > 0.6f)
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"JumpAtk");
				m_AttackCooldown = 0.6f;
			}
			else
			{
				PlayerState->ChangeState(L"Attack1");
				m_AttackCooldown = 0.f;
			}

		}
		// 2단 공격
		else if (KEY_TAP(KEY::X) && m_AtkCount == 2 && m_AttackCooldown < 0.6f)
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"JumpAtk");
				m_AttackCooldown = 0.6f;
			}
			else
			{
				PlayerState->ChangeState(L"Attack2");
				m_AttackCooldown = 0.f;
			}


		}

		// 머리가 파괴되었는지 확인
		if (m_IsHeadless && m_ThrownHead && m_ThrownHead->IsDead())
		{
			m_ThrownHead = nullptr;
			m_IsHeadless = false;
		}

		ThrowHead();
		TeleportToHead();

		// 텔레포트 적용
		if (m_ShouldTeleport)
		{
			vPos = m_TeleportPosition;
			m_ShouldTeleport = false;
		}


	}

	else // 다크팔라딘 모드
	{
		if (m_PrevPos.y < vPos.y) // 현 위치가 전 위치보다 아래에 있을때
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Jump");
			}
			PlayerState->ChangeState(L"Fall");
		}

		else if (m_PrevPos.y > vPos.y) // 현 위치가 전 위치보다 위에 있을때
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Jump");
			}
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			m_lastRightPressed = false;

			vPos.x -= 500.f * DT;

			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Left");
			}
		}

		if (KEY_RELEASED(KEY::LEFT))
		{
			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"LeftIdle");
			}
		}


		if (KEY_PRESSED(KEY::RIGHT))
		{
			m_lastRightPressed = true;
			vPos.x += 500.f * DT;

			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"Right");
			}
		}
		if (KEY_RELEASED(KEY::RIGHT))
		{
			if (m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"RightIdle");
			}
		}

		if (KEY_PRESSED(KEY::UP))
		{
			m_RigidBody->SetGround(true);
			vPos.y -= 200.f * DT;

			if (m_lastRightPressed) // 마지막이 오른쪽이였다면
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"UP", true);
				}
				else
				{
					m_Animator->Play(L"Head_Up_Right", true);
				}
			}
			else
			{
				if (!m_IsHeadless)
				{
					m_Animator->Play(L"UP_Left", true);
				}
				else
				{
					m_Animator->Play(L"Head_Up_Left", true);
				}
			}
		}


		if (KEY_TAP(KEY::DOWN))
		{
			DownOn(true);
			vPos.y += 100.f;
		}

		// 대쉬관련 구현
		if (KEY_TAP(KEY::Z))
		{

			StartDash();
		}

		UpdateDash();

		// 대시 중일 때 이동 적용
		if (m_IsDashing)
		{
			if (m_lastRightPressed) // 오른쪽 눌렸으면
			{
				m_Animator->Play(L"Dark_Dash_Right", true);
			}
			else
			{
				m_Animator->Play(L"Dark_Dash_Left", true);
			}
			vPos += m_DashDirection * (m_DashSpeed * DT);
		}


		// 점프관련 구현
		if (KEY_TAP(KEY::C))
		{
			if (m_DoubleJump == 2)
			{
				SetPos(vPos.x, vPos.y -= 2.f);
				m_RigidBody->Jump();
				--m_DoubleJump;

				Jump_Effect* jump_ef = new Jump_Effect;
				jump_ef->SetPos(vPos.x, vPos.y + 50.f);

				m_Sound = CAssetMgr::Get()->LoadSound(L"Jump", L"sound\\Default_Jump.wav");
				m_Sound->Play();

				CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
				CurLevel->AddObject(jump_ef, LAYER_TYPE::PLAYER_PROJECTILE);
			}
			else if (m_DoubleJump == 1)
			{
				SetPos(vPos.x, vPos.y -= 2.f);
				m_RigidBody->Jump();
				--m_DoubleJump;

				Jump_Effect* jump_ef = new Jump_Effect;
				jump_ef->SetPos(vPos.x, vPos.y + 50.f);

				m_Sound = CAssetMgr::Get()->LoadSound(L"AirJump", L"sound\\Default_Jump_Air.wav");
				m_Sound->Play();

				CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
				CurLevel->AddObject(jump_ef, LAYER_TYPE::PLAYER_PROJECTILE);
			}

		}

		if (m_RigidBody->IsGround())
		{
			m_DoubleJump = 2;
		}


		// 처음 공격
		if (KEY_TAP(KEY::X) && m_AtkCount == 1 && m_AttackCooldown > 0.6f)
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"JumpAtk");
				m_AttackCooldown = 0.6f;
			}
			else
			{
				PlayerState->ChangeState(L"Attack1");
				m_AttackCooldown = 0.f;
			}

		}
		// 2단 공격
		else if (KEY_TAP(KEY::X) && m_AtkCount == 2 && m_AttackCooldown < 0.6f)
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"JumpAtk");
				m_AttackCooldown = 0.6f;
			}
			else
			{
				PlayerState->ChangeState(L"Attack2");
				m_AttackCooldown = 0.f;
			}


		}
		else if (KEY_TAP(KEY::X) && m_AtkCount == 3 && m_AttackCooldown < 0.6f)
		{
			if (!m_RigidBody->IsGround())
			{
				PlayerState->ChangeState(L"JumpAtk");
				m_AttackCooldown = 0.6f;
			}
			else
			{
				PlayerState->ChangeState(L"Attack3");
				m_AttackCooldown = 0.f;
			}
		}

	}

	if (KEY_TAP(KEY::A) && m_Darkporm)
	{
		DarkWave();
		PlayerState->ChangeState(L"Dark_Wave");
	}

	if (KEY_TAP(KEY::S) && m_Darkporm)
	{
		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

		if (m_lastRightPressed)
		{
			Dark_Back* DarkWave = new Dark_Back;
			DarkWave->SetPos(GetPos().x - 30.f, GetPos().y - 30.f);
			DarkWave->SetName(L"Dark_Back");
			CurLevel->AddObject(DarkWave, LAYER_TYPE::PLAYER_PROJECTILE);
		}
		else
		{
			Dark_Back* DarkWave = new Dark_Back;
			DarkWave->SetPos(GetPos().x + 30.f, GetPos().y - 30.f);
			DarkWave->SetName(L"Dark_Back");
			CurLevel->AddObject(DarkWave, LAYER_TYPE::PLAYER_PROJECTILE);
		}
	}


	if (m_AttackCooldown > 0.7f)
	{
		m_AtkCount = 1;
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"Switch", L"sound\\Default_Switch.wav");
		m_Sound->Play();

		m_Darkporm = !m_Darkporm;
	}

	// 카메라 범위 내로 위치 제한
	vPos = CCamera::Get()->PlayerMoveLimit(vPos);

	// 최종 위치 세팅
	SetPos(vPos);

	// 카메라에 플레이어의 상대적 Y 위치 업데이트
	Vec2 playerPos = GetPos();
	CCamera::Get()->SetTargetPos(playerPos);

	m_PrevPos = GetPos();

}

void CPlayer::Render()
{
	m_Animator->Render();
}


void CPlayer::Attack(bool _LastRight)
{
	if (_LastRight) //오른쪽이면
	{
		CPlayer_Normal_Attack* pNormal = new CPlayer_Normal_Attack;
		Vec2 AtkPos = GetPos();
		pNormal->SetName(L"Player_Attack");
		pNormal->SetPos(AtkPos.x + GetScale().x / 2.f, AtkPos.y);
		pNormal->SetCollider(100.f, 100.f);
		CreateObject(pNormal, LAYER_TYPE::PLAYER_PROJECTILE);
	}
	else
	{
		CPlayer_Normal_Attack* pNormal = new CPlayer_Normal_Attack;
		Vec2 AtkPos = GetPos();
		pNormal->SetName(L"Player_Attack");
		pNormal->SetPos(AtkPos.x - GetScale().x / 2.f, AtkPos.y);
		pNormal->SetCollider(100.f, 100.f);
		CreateObject(pNormal, LAYER_TYPE::PLAYER_PROJECTILE);
	}
}

void CPlayer::Dark_Atk(bool _LastRight)
{
	if (_LastRight) //오른쪽이면
	{
		Dark_Attack* pNormal = new Dark_Attack;
		Vec2 AtkPos = GetPos();
		pNormal->SetName(L"Player_Attack");
		pNormal->SetPos(AtkPos.x + GetScale().x / 2.f, AtkPos.y);
		pNormal->SetCollider(300.f, 200.f);

		CAnimator* DarkAni = pNormal->GetDarkAnimator();

		if (m_AtkCount == 1)
		{
			DarkAni->Play(L"Dark_A_Effect_Right", false);
		}
		else if (m_AtkCount == 2)
		{
			DarkAni->Play(L"Dark_B_Effect_Right", false);
		}
		else if (m_AtkCount == 3)
		{
			DarkAni->Play(L"Dark_C_Effect_Right", false);
		}

		CreateObject(pNormal, LAYER_TYPE::PLAYER_PROJECTILE);
	}
	else
	{
		Dark_Attack* pNormal = new Dark_Attack;
		Vec2 AtkPos = GetPos();
		pNormal->SetName(L"Player_Attack");
		pNormal->SetPos(AtkPos.x - GetScale().x / 2.f, AtkPos.y);
		pNormal->SetCollider(300.f, 200.f);

		CAnimator* DarkAni = pNormal->GetDarkAnimator();

		if (m_AtkCount == 1)
		{
			DarkAni->Play(L"Dark_A_Effect_Left", false);
		}
		else if (m_AtkCount == 2)
		{
			DarkAni->Play(L"Dark_B_Effect_Left", false);
		}
		else if (m_AtkCount == 3)
		{
			DarkAni->Play(L"Dark_C_Effect_Left", false);
		}

		CreateObject(pNormal, LAYER_TYPE::PLAYER_PROJECTILE);
	}
}


bool CPlayer::GetGround()
{
	return m_RigidBody->IsGround();
}

void CPlayer::StartDash()
{

	Vec2 vPos = GetPos();

	if ((m_DashCount == 0 || (m_DashCount == 1 && m_CanSecondDash)) && m_DashCooldown <= 0)
	{
		Dash_Effect* Dash_ef = new Dash_Effect(m_lastRightPressed);
		Dash_ef->SetPos(vPos.x, vPos.y + 20.f);

		CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();
		CurLevel->AddObject(Dash_ef, LAYER_TYPE::PLAYER_PROJECTILE);

		if (m_IsDashing)
		{
			// 1단 대시 중 2단 대시 시작
			m_DashProgress = 0.f;

			m_Sound = CAssetMgr::Get()->LoadSound(L"Dash", L"sound\\Default_Dash_DodgeRoll.wav");
			m_Sound->Play();
		}
		else
		{
			// 새로운 대시 시작
			m_IsDashing = true;
			m_DashProgress = 0.f;


			m_Sound = CAssetMgr::Get()->LoadSound(L"Dash", L"sound\\Default_Dash_DodgeRoll.wav");
			m_Sound->Play();
		}

		m_DashCount++;
		m_CanSecondDash = false;

		// 대시 방향 설정 (마지막으로 누른 방향키에 따라)
		m_DashDirection = m_lastRightPressed ? Vec2(1, 0) : Vec2(-1, 0);

		// 리지드바디 영향 제거 (선택적)
		if (m_RigidBody)
		{
			m_RigidBody->SetVelocity(Vec2(0, 0));
			m_RigidBody->UseGravity(false);
		}

		// 첫 번째 대시일 경우 타이머 시작
		if (m_DashCount == 1)
		{
			m_DashInputTimer = 0.f;
		}
	}

}

void CPlayer::UpdateDash()
{

	if (m_IsDashing)
	{
		float dashStep = m_DashSpeed * DT;
		m_DashProgress += dashStep;

		if (m_DashProgress >= m_DashDistance)
		{
			if (m_DashCount == 1)
			{
				// 1단 대시 종료, 2단 대시 가능 상태로 전환
				m_IsDashing = false;
				m_CanSecondDash = true;
				m_RigidBody->UseGravity(true);
			}
			else
			{
				// 2단 대시 종료
				m_IsDashing = false;
				m_DashCooldown = 1.5f; // 대시 쿨다운 시간 설정
				m_CanSecondDash = false;
				m_DashCount = 0;

				// 리지드바디 영향 복구 (선택적)
				if (m_RigidBody)
				{
					m_RigidBody->UseGravity(true);
				}
			}
		}
	}

	else if (m_DashCount == 1)
	{
		m_DashInputTimer += DT;
		if (m_DashInputTimer >= m_DashInputWindow)
		{
			// 0.6초 내에 두 번째 대시 입력이 없었음
			m_DashCooldown = 1.5f;
			m_DashCount = 0;
			m_CanSecondDash = false;
		}
	}

	else if (m_DashCooldown > 0)
	{
		m_DashCooldown -= DT;
		if (m_DashCooldown <= 0)
		{
			m_DashCount = 0;
			m_CanSecondDash = false;
		}
	}
}

void CPlayer::ThrowHead()
{
	if (!m_IsHeadless && KEY_TAP(KEY::A))
	{
		m_ThrownHead = new CSkullHead;
		Vec2 playerPos = GetPos();
		Vec2 direction;

		// 플레이어가 바라보는 방향으로 머리를 던집니다
		if (m_lastRightPressed)
		{
			direction = Vec2(1.0f, 0.0f);
			m_Animator->Play(L"Skul_Skil_Right", false);
			m_ThrownHead->GetAnimaor()->Play(L"Skul_Head_Right", true);
		}

		else
		{
			direction = Vec2(-1.0f, 0.0f);
			m_Animator->Play(L"Skul_Skil_Left", false);
			m_ThrownHead->GetAnimaor()->Play(L"Skul_Head_Left", true);
		}

		m_Sound = CAssetMgr::Get()->LoadSound(L"Throwing", L"sound\\Skul_SkullThrowing.wav");
		m_Sound->Play();


		m_ThrownHead->SetPos(playerPos);
		m_ThrownHead->SetDirection(direction);
		CreateObject(m_ThrownHead, LAYER_TYPE::PLAYER_PROJECTILE);

		m_IsHeadless = true;
	}
}

void CPlayer::DarkWave()
{
	CLevel* CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	if (m_lastRightPressed)
	{
		Dark_Waves* DarkWave = new Dark_Waves(m_lastRightPressed);
		DarkWave->SetPos(GetPos().x + 40.f, GetPos().y);
		DarkWave->SetName(L"Player_Attack");
		CurLevel->AddObject(DarkWave, LAYER_TYPE::PLAYER_PROJECTILE);
	}
	else
	{
		Dark_Waves* DarkWave = new Dark_Waves(m_lastRightPressed);
		DarkWave->SetPos(GetPos().x - 40.f, GetPos().y);
		DarkWave->SetName(L"Player_Attack");
		CurLevel->AddObject(DarkWave, LAYER_TYPE::PLAYER_PROJECTILE);
	}

}


void CPlayer::TeleportToHead()
{
	if (m_IsHeadless && m_ThrownHead && KEY_TAP(KEY::S))
	{
		// m_ThrownHead가 유효한 객체인지 확인
		if (m_ThrownHead->IsDead())
		{
			m_ThrownHead = nullptr;
			m_IsHeadless = false;
			return;
		}

		m_Sound = CAssetMgr::Get()->LoadSound(L"Teleport", L"sound\\Common_Teleport.wav");
		m_Sound->Play();

		m_TeleportPosition = m_ThrownHead->GetPosition();
		Vec2 pPos = GetPos();

		if (pPos.x < m_TeleportPosition.x)
		{
			m_Animator->Play(L"IDLE_Right", true);
		}

		else
		{
			m_Animator->Play(L"IDLE_Left", true);
		}

		m_ShouldTeleport = true;

		DeleteObject(m_ThrownHead);
		m_ThrownHead = nullptr;
		m_IsHeadless = false;
	}
}

void CPlayer::Player_Hit()
{
	Vec2 pPos = GetPos();
	SetPos(pPos.x, pPos.y - 2.f);
	m_RigidBody->Hit();
}

void CPlayer::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherCollider->GetName() == L"Platform")
	{

	}


}

void CPlayer::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"floor")
	{
		int a = 0;
		if (m_Floor)
		{
			m_RigidBody->SetGround(false);

			DownOn(false);
		}
	}

	if (_OtherCollider->GetName() == L"Platform")
	{
		int a = 0;
	}

}


