#pragma once
#include "CObj.h"

class CTexture;
class Boss_HP;

class Boss_Monster;


class Boss_Name :
    public CObj
{
public:
    CTexture* m_Texture;

    Boss_HP* m_BossHpBar;
    Boss_Monster* m_Boss;


public:

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Boss_Name);
    Boss_Name(Vec2 _Pos);
    Boss_Name(const Boss_Name& _Other);
    ~Boss_Name();
};
