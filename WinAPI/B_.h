#pragma once
#include "CObj.h"

class CTexture;

class B_ :
    public CObj
{
public:
    CTexture* m_PlayerImge;
public:

    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(B_);
    B_();
    B_(const B_& _Other);
    ~B_();
};