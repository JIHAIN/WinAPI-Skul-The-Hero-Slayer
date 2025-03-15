#include "pch.h"
#include "CMissile.h"

#include "CTimeMgr.h"
#include "CEngine.h"
#include "CCollider.h"

CMissile::CMissile()
	: m_Velocity(0.f, 0.f)
	, m_Mass(1.f)
{
	CCollider* pCollider = AddComponent(new CCollider);
	pCollider->SetScale(Vec2(20.f, 20.f));
}

CMissile::~CMissile()
{
}

void CMissile::Tick()
{
	Vec2 vPos = GetPos();

	vPos += m_Velocity * DT;

	SetPos(vPos);
}

void CMissile::Render()
{
	

	Vec2 vPos = GetRenderPos();
	Vec2 vScale = GetScale();
	SELECT_BRUSH(BackDC, BRUSH_TYPE::RED);

	Ellipse(BackDC, vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f)
		, vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));

	// 라인 그리기
	//SELECT_PEN(dc, PEN_TYPE::MAGENTA);
	//Vec2 vEnd = vPos + m_Velocity / 4.f;
	//MoveToEx(dc, vPos.x, vPos.y, nullptr);
	//LineTo(dc, vEnd.x, vEnd.y);
}

void CMissile::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	//DeleteObject(this);
}
