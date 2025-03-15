#pragma once
#include "CComponent.h"

class CCollider :
    public CComponent
{
private:
    Vec2    m_OffsetPos;
    Vec2    m_FinalPos;
    Vec2    m_Scale;

    int     m_OverlapCount;

public:
    void SetOffset(Vec2 _Offset) { m_OffsetPos = _Offset; }
    void SetScale(Vec2 _Scale) { m_Scale = _Scale; }

    Vec2 GetFinalPos() { return m_FinalPos; }
    Vec2 GetScale() { return m_Scale; }

public:
    virtual void FinalTick() override;

    void BeginOverlap(CCollider* _Other);
    void Overlap(CCollider* _Other);
    void EndOverlap(CCollider* _Other);



public:
    CLONE(CCollider);
    CCollider();
    CCollider(const CCollider& _Other);
    ~CCollider();
};

