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
	, m_HpBarPos(Vec2(97, 749))  // HP �� ��ġ ����
	, m_HpBarSize(Vec2(268, 23)) // HP �� ũ�� ����
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

	// HP �� ������ ��ġ �� ũ�� ����
	int barPosX = 97;
	int barPosY = 749;
	int barWidth = 268;  // HP ���� �ִ� �ʺ�
	int barHeight = 23;  // HP ���� ����

	// �÷��̾��� ���� HP ���� ��� (�ε��Ҽ��� ���)
	float currentHP = static_cast<float>(m_Player->GetHP());
	float maxHP = static_cast<float>(m_Player->GetMaxHP());
	float hpRatio = currentHP / maxHP;

	// ���� HP�� ���� ���� �ʺ� ���
	int currentHpWidth = static_cast<int>(barWidth * hpRatio + 0.5f); // �ݿø�

	// ���� HP �� ������
	BitBlt(BackDC,
		barPosX, barPosY,
		currentHpWidth, barHeight,
		m_PlayerHpBar->GetDC(),
		0, 0,
		SRCCOPY);

}