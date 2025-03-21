#include "pch.h"
#include "CCollider.h"

#include "CDebugMgr.h"
#include "CCollisionMgr.h"

#include "CCamera.h"

CCollider::CCollider()
	: CComponent(COMPONENT_TYPE::COLLIDER)
	, m_OverlapCount(0)
{
}

CCollider::CCollider(const CCollider& _Other)
	: CComponent(_Other)
	, m_OffsetPos(_Other.m_OffsetPos)
	, m_Scale(_Other.m_Scale)
	, m_OverlapCount(0)
{
}

CCollider::~CCollider()
{
}

void CCollider::FinalTick()
{
	// 충돌체의 최종 위치 계산
	m_FinalPos = GetOwner()->GetPos() + m_OffsetPos;



	// 충돌체를 레이어 번호에 맞게 충돌매니저에 등록
	CCollisionMgr::Get()->RegisterCollider(this, (LAYER_TYPE)GetOwner()->GetLayerType());

	// 충돌체를 디버그 렌더링 요청
	Vec2 vRenderPos = CCamera::Get()->GetRenderPos(m_FinalPos);

	if(m_OverlapCount)
		DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::RED, BRUSH_TYPE::HOLLOW, vRenderPos, m_Scale);
	else
		DrawDebugShape(DEBUG_SHAPE::RECT, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, vRenderPos, m_Scale);
}

void CCollider::BeginOverlap(CCollider* _Other)
{
	++m_OverlapCount;

	GetOwner()->BeginOverlap(this, _Other->GetOwner(), _Other);	
}

void CCollider::Overlap(CCollider* _Other)
{
	GetOwner()->Overlap(this, _Other->GetOwner(), _Other);
}

void CCollider::EndOverlap(CCollider* _Other)
{
	--m_OverlapCount;

	GetOwner()->EndOverlap(this, _Other->GetOwner(), _Other);	
}
