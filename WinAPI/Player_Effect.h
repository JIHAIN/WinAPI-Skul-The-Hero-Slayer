#pragma once
#include "CObj.h"
class CAnimator;
class CPlayer;

class Player_Effect :
    public CObj
{
private:
    CAnimator* m_Animator;
    CPlayer* m_Player;
    float LifeTime;
    float Duration;


public:
    CAnimator* GetEditAnimator() { return m_Animator; }
    void SetDuration(float _Duration) { Duration = _Duration; }

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Player_Effect);
    Player_Effect();
    Player_Effect(const Player_Effect& _Other);
    ~Player_Effect();
};
