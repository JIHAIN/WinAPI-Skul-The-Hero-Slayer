#include "pch.h"
#include "Boss_HP.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "Boss_Monster.h"
#include "CLevel.h"
#include "CLevelMgr.h"


Boss_HP::Boss_HP()
	: m_PlayerHpBar(nullptr)
	, m_Boss(nullptr)
	, m_HpBarPos(Vec2(35, 200))  // HP 바 위치 설정
	, m_HpBarSize(Vec2(275, 12)) // HP 바 크기 설정
{
	m_Boss = CLevelMgr::Get()->GetCurrentLevel()->GetBoss();
	m_PlayerHpBar = CAssetMgr::Get()->LoadTexture(L"Boss_HpBar", L"texture\\Boss_HpBar.png");
}

Boss_HP::Boss_HP(const Boss_HP& _Other)
{
}


Boss_HP::~Boss_HP()
{
}



void Boss_HP::Render()
{
	if (m_PlayerHpBar == nullptr)
	{
		return;
	}

	if (m_Boss->IsDead() || m_Boss == nullptr)
	{
		DeleteObject(this);
	}


	// HP 바 렌더링 위치 및 크기 설정

	Vec2 myPos = GetPos();
	int barPosX = 30;
	int barPosY = 72;
	int barWidth = 550;  // HP 바의 최대 너비
	int barHeight = 24;  // HP 바의 높이

	// 플레이어의 현재 HP 비율 계산 (부동소수점 사용)
	float currentHP = static_cast<float>(m_Boss->GetMonsterInfo().HP);
	float maxHP = static_cast<float>(m_Boss->GetMonsterInfo().MaxHP);
	float hpRatio = currentHP / maxHP;

	// 현재 HP에 따른 바의 너비 계산
	int currentHpWidth = static_cast<int>(barWidth * hpRatio + 0.5f); // 반올림

	// 현재 HP 바 렌더링
	BitBlt(BackDC,
		myPos.x + barPosX, myPos.y + barPosY,
		currentHpWidth, barHeight,
		m_PlayerHpBar->GetDC(),
		0, 0,
		SRCCOPY);

}