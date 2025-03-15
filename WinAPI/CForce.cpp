#include "pch.h"
#include "CForce.h"

#include "CDebugMgr.h"
#include "CTimeMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CObj.h"
#include "CRigidBody.h"


CForce::CForce()
	: m_Force(0.f)
	, m_Range(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
{
}

CForce::CForce(const CForce& _Other)
	: CObj(_Other)
	, m_Force(_Other.m_Force)
	, m_Range(_Other.m_Range)
	, m_Duration(_Other.m_Duration)
	, m_AccTime(0.f)
{
}

CForce::~CForce()
{
}

void CForce::Tick()
{
	CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();

	for (int i = 0; i < 32; ++i)
	{
		const vector<CObj*>& vecObjects = pLevel->GetLayer((LAYER_TYPE)i);

		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			float fDist = GetPos().Distance(vecObjects[j]->GetPos());

			if (m_Range < fDist)
				continue;

			CRigidBody* pRigidBody = vecObjects[j]->GetComponent<CRigidBody>();
			if (nullptr == pRigidBody)
				continue;

			// 힘이 적용될 방향을 구한다.
			Vec2 ForceDir = vecObjects[j]->GetPos() - GetPos();
			ForceDir.Normalize();

			// 거리에 따라서 힘이 감소한다.
			float ForceRatio = 1.f - (fDist / m_Range);
			if (ForceRatio < 0.f)
				ForceRatio = 0.f;

			Vec2 vForce = ForceDir * m_Force * ForceRatio;

			// 해당 방향으로 Force 의 크기만큼 키워서 상대방 RigidBody 에 적용시킨다.
			pRigidBody->AddForce(vForce);
		}
	}
	

	DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW, GetPos(), Vec2(m_Range * 2.f, m_Range * 2.f));

	m_AccTime += DT;

	if (m_Duration < m_AccTime)
		DeleteObject(this);
}