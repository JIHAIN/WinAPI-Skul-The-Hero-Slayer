#pragma once
#include "CObj.h"

class CTexture;
class Boss_Monster;

class Boss_HP :
    public CObj
{
public:
    CTexture* m_PlayerHpBar;
    Boss_Monster* m_Boss;

    Vec2      m_HpBarPos;  // HP �� ��ġ
    Vec2      m_HpBarSize; // HP �� ũ��

public:
    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(Boss_HP);
    Boss_HP();
    Boss_HP(const Boss_HP& _Other);
    ~Boss_HP();
};