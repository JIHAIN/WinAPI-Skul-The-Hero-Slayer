#pragma once
#include "CObj.h"

class CTexture;

class A_ :
    public CObj
{
public:
    CTexture* m_PlayerImge;
public:

    virtual void Tick() override {};
    virtual void Render() override;

public:
    CLONE(A_);
    A_();
    A_(const A_& _Other);
    ~A_();
};