#pragma once
#include "CObj.h"
class CTexture;
class CPlayer;


class Player_Skill :
    public CObj
{
public:
    CTexture* m_PlayerImge;

    CPlayer* m_Player;


public:

    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(Player_Skill);
    Player_Skill();
    Player_Skill(const Player_Skill& _Other);
    ~Player_Skill();
};
