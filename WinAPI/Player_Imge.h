#pragma once
#include "CObj.h"

class CTexture;
class CPlayer;

class Player_Imge :
    public CObj
{
public:
    CTexture* m_PlayerImge;

    CPlayer* m_Player;

public:

    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(Player_Imge);
    Player_Imge();
    Player_Imge(const Player_Imge& _Other);
    ~Player_Imge();
};
