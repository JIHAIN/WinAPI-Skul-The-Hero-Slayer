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

CObj::CObj(const CObj& _Other) //���� ������
	:	CBase(_Other) // �� �θ����� �θ𳢸� �ϵ��� ����������
	,   m_Pos(_Other.m_Pos)
	,	m_Scale(_Other.m_Scale)
	,	m_LayerType(-1) // ���� ���� ���̾ �ƴ� �� ������ ���� ����
	,	m_bDead(false)  // �׽�ũ �Ŵ����� ���ؼ� �����޾ƾ鸶
{
	// ������Ʈ�� ���� ��������� ���ؼ� �Ȱ��� ������Ʈ�� �޾��ش�.
	// AddComponent �� ���ʵ� �˷��ְ� Ǫ���鵵 ����
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
		if (m_vecComponent[i]->GetComponentType() == COMPONENT_TYPE::COLLIDER) // �ݶ��̴� ������Ʈ�� ã�Ƽ�
		{
			// dynamic_cast�� ����Ͽ� CCollider Ÿ������ ĳ����
			CCollider* pCollider = dynamic_cast<CCollider*>(m_vecComponent[i]);
			// ĳ������ �����ϸ� pCollider�� nullptr�� �ƴϹǷ�, GetScale() �Լ��� ȣ��
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