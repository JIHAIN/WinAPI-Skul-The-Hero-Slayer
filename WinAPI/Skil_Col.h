#pragma once
#include "CObj.h"

class CAnimator;


class Skil_Col :
    public CObj
{
public:
    CAnimator* m_Animator;
    float AccTime;
public:

    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(Skil_Col);
    Skil_Col();
    Skil_Col(const Skil_Col& _Other);
    ~Skil_Col();
};

