#include "pch.h"
#include "CObj.h"

#include "CEngine.h"
#include "CTimeMgr.h"

#include "CComponent.h"
#include "CCollider.h"



CObj::CObj()
	: m_LayerType(-1)
	, m_bDead(false)
{
}

CObj::~CObj()
{
	Release_Vector(m_vecComponent);
}

CObj::CObj(const CObj& _Other) //복사 생성자
	:	CBase(_Other) // 꼭 부모쪽은 부모끼리 하도록 명시해줘야함
	,   m_Pos(_Other.m_Pos)
	,	m_Scale(_Other.m_Scale)
	,	m_LayerType(-1) // 나랑 같은 레이어가 아닐 수 있으니 값을 안줌
	,	m_bDead(false)  // 테스크 매니저를 통해서 결정받아얗마
{
	// 컴포넌트들 끼리 복사생성을 통해서 똑같은 컴포넌트를 받아준다.
	// AddComponent 가 오너도 알려주고 푸쉬백도 해줌
	for (size_t i = 0; i < _Other.m_vecComponent.size(); ++i)
	{
		CComponent* pClonedComponent = _Other.m_vecComponent[i]->Clone();
		AddComponent(pClonedComponent);
	}

}


Vec2 CObj::GetRenderPos()
{
	return CCamera::Get()->GetRenderPos(m_Pos);
}

Vec2 CObj::GetScale()
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		if (m_vecComponent[i]->GetComponentType() == COMPONENT_TYPE::COLLIDER) // 콜라이더 컴포넌트를 찾아서
		{
			// dynamic_cast를 사용하여 CCollider 타입으로 캐스팅
			CCollider* pCollider = dynamic_cast<CCollider*>(m_vecComponent[i]);
			// 캐스팅이 성공하면 pCollider는 nullptr이 아니므로, GetScale() 함수를 호출
			if (pCollider)
			{
				return pCollider->GetScale();
			}
		}
		return Vec2(0.f, 0.f);
	}

	return Vec2(0.f, 0.f);
}

CComponent* CObj::GetComponent(COMPONENT_TYPE _Type)
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		if (m_vecComponent[i]->GetComponentType() == _Type)
			return m_vecComponent[i];
	}

	return nullptr;
}

CComponent* CObj::GetComponentByName(const wstring& _Name)
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		if (m_vecComponent[i]->GetName() == _Name)
		{
			return m_vecComponent[i];
		}
	}

	return nullptr;
}

void CObj::FinalTick()
{
	for (size_t i = 0; i < m_vecComponent.size(); ++i)
	{
		m_vecComponent[i]->FinalTick();
	}
}

void CObj::Render()
{
	HDC dc = CEngine::Get()->GetBackDC();

	SELECT_PEN(dc, PEN_TYPE::RED);
	SELECT_BRUSH(dc, BRUSH_TYPE::BLUE);	


	Vec2 vRenderPos = CCamera::Get()->GetRenderPos(m_Pos);

	Rectangle(dc, vRenderPos.x - (m_Scale.x / 2.f), vRenderPos.y - (m_Scale.y / 2.f)
				, vRenderPos.x + (m_Scale.x / 2.f), vRenderPos.y + (m_Scale.y / 2.f));
}