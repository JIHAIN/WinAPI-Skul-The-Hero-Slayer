#include "pch.h"
#include "HP_Bar.h"
#include "CTexture.h"
#include "CEngine.h"
#include "CAssetMgr.h"
#include "CCollider.h"

#include "CPlayer.h"
#include "CLevel.h"
#include "CLevelMgr.h"


HP_Bar::HP_Bar()
	: m_PlayerHpBar(nullptr)
	, m_Player(nullptr)
	, m_HpBarPos(Vec2(97, 749))  // HP 바 위치 설정
	, m_HpBarSize(Vec2(268, 23)) // HP 바 크기 설정
{
	m_Player = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	m_PlayerHpBar = CAssetMgr::Get()->LoadTexture(L"PlayerHpBar", L"texture\\HP_Bar.png");
}

HP_Bar::HP_Bar(const HP_Bar& _Other)
{
}


HP_Bar::~HP_Bar()
{
}

void HP_Bar::Render()
{
	if (m_PlayerHpBar == nullptr)
	{
		return;
	}

	// HP 바 렌더링 위치 및 크기 설정
	int barPosX = 97;
	int barPosY = 749;
	int barWidth = 268;  // HP 바의 최대 너비
	int barHeight = 23;  // HP 바의 높이

	// 플레이어의 현재 HP 비율 계산 (부동소수점 사용)
	float currentHP = static_cast<float>(m_Player->GetHP());
	float maxHP = static_cast<float>(m_Player->GetMaxHP());
	float hpRatio = currentHP / maxHP;

	// 현재 HP에 따른 바의 너비 계산
	int currentHpWidth = static_cast<int>(barWidth * hpRatio + 0.5f); // 반올림

	// 현재 HP 바 렌더링
	BitBlt(BackDC,
		barPosX, barPosY,
		currentHpWidth, barHeight,
		m_PlayerHpBar->GetDC(),
		0, 0,
		SRCCOPY);

}