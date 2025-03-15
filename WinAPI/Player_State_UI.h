#pragma once
#include "CObj.h"

class CTexture;
class HP_Bar;
class Player_Imge;
class CPlayer;
class Player_Skill;
class Player_Skill2;
class A_;
class B_;
class CLevel;

class Player_State_UI :
    public CObj
{
public:
    CTexture* m_Texture;
    CLevel* CurLevel;
    Player_Imge* m_PlayerImge;
    HP_Bar* m_PlayerHpBar;
    CPlayer* m_Player;
    Player_Skill* m_Skill;
    Player_Skill2* m_Skill2;
    A_* A1;
    B_* B1;

    Vec2 APos;
    Vec2 BPos;

public:

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Player_State_UI);
    Player_State_UI(Vec2 _Pos);
    Player_State_UI(const Player_State_UI& _Other);
    ~Player_State_UI();
};
