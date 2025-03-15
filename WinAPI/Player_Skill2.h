#pragma once
#include "CObj.h"
class CTexture;
class CPlayer;


class Player_Skill2 :
    public CObj
{
public:
    CTexture* m_PlayerImge;


    CPlayer* m_Player;

public:

    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(Player_Skill2);
    Player_Skill2();
    Player_Skill2(const Player_Skill2& _Other);
    ~Player_Skill2();
};
