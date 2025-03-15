#pragma once
#include "CUI.h"


class CPanel :
    public CUI
{
private:
    Vec2   m_GrabPos;


public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;

    virtual void BeginHovered() override;

public:
    virtual void LBtnDown();


public:
    CLONE(CPanel); // 딱히 깊은 복사를 해줄건 없어서 패스
    CPanel();
    ~CPanel();
};

