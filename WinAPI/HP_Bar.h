#pragma once
#include "CObj.h"

class CTexture;
class CPlayer;

class HP_Bar :
    public CObj
{
public:
    CTexture* m_PlayerHpBar;
    CPlayer* m_Player;

    Vec2      m_HpBarPos;  // HP �� ��ġ
    Vec2      m_HpBarSize; // HP �� ũ��

public:
    virtual void Tick() override { int a = 0; }
    virtual void Render() override;

public:
    CLONE(HP_Bar);
    HP_Bar();
    HP_Bar(const HP_Bar& _Other);
    ~HP_Bar();
};