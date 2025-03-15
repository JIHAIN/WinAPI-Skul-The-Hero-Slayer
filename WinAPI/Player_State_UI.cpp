#include "pch.h"
#include "Player_State_UI.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "Player_Imge.h"
#include "HP_Bar.h"
#include "Player_Skill.h"
#include "Player_Skill2.h"
#include "A_.h"
#include "B_.h"
#include "CKeyMgr.h"
#include "Skil_Col.h"


Player_State_UI::Player_State_UI(Vec2 _Pos)
	: m_Texture(nullptr)
	, m_PlayerHpBar(nullptr)
	, m_PlayerImge(nullptr)
	, m_Player(nullptr)
	, CurLevel(nullptr)
	, APos(0, 0)
	, BPos(0, 0)
{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	m_Texture = CAssetMgr::Get()->LoadTexture(L"Player_State_UI", L"texture\\Player_Normal_Frame.png");

	CurLevel = CLevelMgr::Get()->GetCurrentLevel();

	Vec2 myPos = GetRenderPos();

	m_PlayerImge = new Player_Imge;
	m_PlayerImge->SetPos(_Pos.x + 25.f, _Pos.y + 10.f);
	CurLevel->AddObject(m_PlayerImge, LAYER_TYPE::MYUIUP);

	m_PlayerHpBar = new HP_Bar;
	m_PlayerHpBar->SetPos(_Pos);
	CurLevel->AddObject(m_PlayerHpBar, LAYER_TYPE::MYUIUP);

	m_Skill = new Player_Skill;
	m_Skill->SetPos(_Pos.x + 128.f, _Pos.y + 32.f);
	CurLevel->AddObject(m_Skill, LAYER_TYPE::MYUIUP);
	APos = Vec2(myPos.x + _Pos.x + 128.f, myPos.y + _Pos.y + 32.f);

	m_Skill2 = new Player_Skill2;
	m_Skill2->SetPos(_Pos.x + 206.f, _Pos.y + 32.f);
	CurLevel->AddObject(m_Skill2, LAYER_TYPE::MYUIUP);
	BPos = Vec2(myPos.x + _Pos.x + 206.f, myPos.y + _Pos.y + 32.f);

	A1 = new A_;
	A1->SetPos(_Pos.x + 142.f, _Pos.y + 15);
	CurLevel->AddObject(A1, LAYER_TYPE::MYUIUP);


	B1 = new B_;
	B1->SetPos(_Pos.x + 220.f, _Pos.y + 15);
	CurLevel->AddObject(B1, LAYER_TYPE::MYUIUP);

}

Player_State_UI::Player_State_UI(const Player_State_UI& _Other)
{
}


Player_State_UI::~Player_State_UI()
{
}

void Player_State_UI::Tick()
{
	if (KEY_TAP(KEY::A))
	{
		Skil_Col* skil = new Skil_Col;
		skil->SetPos(128 + 28.f, 682 + 28.f);
		CurLevel->AddObject(skil, LAYER_TYPE::MYUIUP);
	}

	if (KEY_TAP(KEY::S))
	{
		Skil_Col* skil = new Skil_Col;
		skil->SetPos(206 + 28.f, 682 + 28.f);
		CurLevel->AddObject(skil, LAYER_TYPE::MYUIUP);
	}

}

void Player_State_UI::Render()
{

	if (m_Texture == nullptr)
	{
		return;
	}

	Vec2 vPos = GetPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vPos.x, (int)vPos.y
		, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);

}