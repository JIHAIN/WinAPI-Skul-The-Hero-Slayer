#include "pch.h"
#include "CPlatform.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCollider.h"
#include "CRigidBody.h"

#include "CSound.h"
#include "CAssetMgr.h"

CPlatform::CPlatform()
	: m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
}

CPlatform::CPlatform(const CPlatform& _Other)
	: CObj(_Other)
	, m_Collider(nullptr)
{
	m_Collider = GetComponent<CCollider>();
	m_Collider->SetName(L"Platform");
}

CPlatform::~CPlatform()
{
}

void CPlatform::SetScale(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void CPlatform::SetScale(float _Width, float _Height)
{
	float tileSize = 64;

	CObj::SetScale(tileSize * _Width, tileSize * _Height);

	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(tileSize * _Width, tileSize * _Height -35.f));
	}
}

void CPlatform::SetScaleMouse(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void CPlatform::Tick()
{

}

void CPlatform::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();
	// �� �浹ü�� �� �ະ �Ÿ���
	vDiff.x = abs(vDiff.x);
	vDiff.y = abs(vDiff.y);
	// �� �浹ü�� ũ�⸦ ���ݾ� ��ģ ��
	float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
	float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;
	// �浹 ���� ���
	float overlapX = ScaleX - vDiff.x;
	float overlapY = ScaleY - vDiff.y;
	Vec2 CurPos = _OtherObj->GetPos();
	Vec2 myPos = _OwnCollider->GetFinalPos();
	Vec2 mySclae = _OwnCollider->GetScale();
	Vec2 PlaPos = _OtherCollider->GetFinalPos();
	Vec2 PlaScale = _OtherCollider->GetScale();
	// ��� �ݶ��̴��� min ����, �Ʒ� : max ������, ��
	Vec2 PlaMin = Vec2(PlaPos.x - PlaScale.x / 2.f, PlaPos.y + PlaScale.y / 2.f);
	Vec2 PlaMax = Vec2(PlaPos.x + PlaScale.x / 2.f, PlaPos.y - PlaScale.y / 2.f);
	// �� �ݶ��̴��� min ����, �Ʒ� : max ������, ��
	Vec2 myMin = Vec2(myPos.x - mySclae.x / 2.f, myPos.y + mySclae.y / 2.f);
	Vec2 myMax = Vec2(myPos.x + mySclae.x / 2.f, myPos.y - mySclae.y / 2.f);

	if (pOtherObj)
	{
		if (myMax.y > PlaMin.y&& PlaMax.y > myMax.y)
		{
			// �÷��̾ ������ �Ʒ����� ���� �׶��� ����
			CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
			pRigidBody->SetGround(false);
		}
		else
		{
			CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
			pRigidBody->SetGround(true);
		}
		
		CSound* m_Sound = CAssetMgr::Get()->LoadSound(L"Landing", L"sound\\Default_Landing.wav");
		m_Sound->Play();

	}

	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(true);

		Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();

		// �� �浹ü�� �� �ະ �Ÿ���
		vDiff.x = abs(vDiff.x);
		vDiff.y = abs(vDiff.y);

		// �� �浹ü�� ũ�⸦ ���ݾ� ��ģ ��
		float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
		float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

		// �浹 ���� ���
		float overlapX = ScaleX - vDiff.x;
		float overlapY = ScaleY - vDiff.y;

		Vec2 CurPos = _OtherObj->GetPos();
		Vec2 myPos = _OwnCollider->GetFinalPos();
		Vec2 mySclae = _OwnCollider->GetScale();
		Vec2 PlaPos = _OtherCollider->GetFinalPos();
		Vec2 PlaScale = _OtherCollider->GetScale();

		pOtherObj2->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y));

	}
}

Vec2 CPlatform::CalculateCollisionNormal(CCollider* _OwnCollider, CCollider* _OtherCollider)
{
	Vec2 platformPos = _OwnCollider->GetFinalPos();
	Vec2 platformScale = _OwnCollider->GetScale();
	Vec2 playerPos = _OtherCollider->GetFinalPos();
	Vec2 playerScale = _OtherCollider->GetScale();

	Vec2 distance = playerPos - platformPos;

	// �浹 ���� ���
	float overlapX = (platformScale.x / 2 + playerScale.x / 2) - abs(distance.x);
	float overlapY = (platformScale.y / 2 + playerScale.y / 2) - abs(distance.y);

	// �� ���� �������� ���� ���� �浹 ����
	if (overlapX < overlapY)
	{
		return Vec2(distance.x > 0 ? 1 : -1, 0);
	}
	else
	{
		return Vec2(0, distance.y > 0 ? 1 : -1);
	}
}

// ���� ������
void CPlatform::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	if (_OtherObj->GetName() == L"Player")
	{

		CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
		
		Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();

		// �� �浹ü�� �� �ະ �Ÿ���
		vDiff.x = abs(vDiff.x);
		vDiff.y = abs(vDiff.y);

		// �� �浹ü�� ũ�⸦ ���ݾ� ��ģ ��
		float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
		float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

		// �浹 ���� ���
		float overlapX = ScaleX - vDiff.x;
		float overlapY = ScaleY - vDiff.y;

		Vec2 CurPos = _OtherObj->GetPos();
		Vec2 myPos = _OwnCollider->GetFinalPos();
		Vec2 mySclae = _OwnCollider->GetScale();
		Vec2 PlaPos = _OtherCollider->GetFinalPos();
		Vec2 PlaScale = _OtherCollider->GetScale();

		// ��� �ݶ��̴��� min ����, �Ʒ� : max ������, ��
		Vec2 PlaMin = Vec2(PlaPos.x - PlaScale.x / 2.f, PlaPos.y + PlaScale.y / 2.f);
		Vec2 PlaMax = Vec2(PlaPos.x + PlaScale.x / 2.f, PlaPos.y - PlaScale.y / 2.f);

		// �� �ݶ��̴��� min ����, �Ʒ� : max ������, ��
		Vec2 myMin = Vec2(myPos.x - mySclae.x / 2.f, myPos.y + mySclae.y / 2.f);
		Vec2 myMax = Vec2(myPos.x + mySclae.x / 2.f, myPos.y - mySclae.y / 2.f);

		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();

		// ���� �����Ӱ� ���� ������ ��� ���� �־��ٸ� ��ġ ���� ���� ����
		if (pRigidBody->IsGround() && pRigidBody->WasGround())
		{
			return;
		}

		// �浹 ���̰� �� ���� �࿡ ���� ���� ó��
		if (overlapX < overlapY)
		{
			if (myMax.x < PlaMax.x && myMax.x > PlaMin.x)  //�÷��̾ ������ �����ʿ��� �ٰ����� �׶��嵵 ����
			{
				_OtherObj->SetPos(CurPos.x += (ScaleX - vDiff.x), CurPos.y);

				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				
				}
			}
			else
			{
				// �÷��̾ ������ ���ʿ��� ���� �׶��� ����
				_OtherObj->SetPos(CurPos.x -= (ScaleX - vDiff.x), CurPos.y);
				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				}
			}
		}
		else
		{
			if (myMax.y < PlaMin.y && PlaMax.y < myMax.y) //�÷��̾ ������ �� 
			{
				_OtherObj->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y) - 1.f);

				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(true);
				}
			}
			else
			{
				// �÷��̾ ������ �Ʒ����� ���� �׶��� ����
				_OtherObj->SetPos(CurPos.x, CurPos.y += (ScaleY - vDiff.y));
				if (pOtherObj)
				{
					CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
					pRigidBody->SetGround(false);
				}
			}
		}
	}

	if (_OtherObj->GetName() == L"Monster")
	{
		CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);
		Vec2 vDiff = _OwnCollider->GetFinalPos() - _OtherCollider->GetFinalPos();

		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(true);

		// �� �浹ü�� �� �ະ �Ÿ���
		vDiff.x = abs(vDiff.x);
		vDiff.y = abs(vDiff.y);

		// �� �浹ü�� ũ�⸦ ���ݾ� ��ģ ��
		float ScaleX = _OwnCollider->GetScale().x / 2.f + _OtherCollider->GetScale().x / 2.f;
		float ScaleY = _OwnCollider->GetScale().y / 2.f + _OtherCollider->GetScale().y / 2.f;

		// �浹 ���� ���
		float overlapX = ScaleX - vDiff.x;
		float overlapY = ScaleY - vDiff.y;

		Vec2 CurPos = _OtherObj->GetPos();
		Vec2 myPos = _OwnCollider->GetFinalPos();
		Vec2 mySclae = _OwnCollider->GetScale();
		Vec2 PlaPos = _OtherCollider->GetFinalPos();
		Vec2 PlaScale = _OtherCollider->GetScale();

		pOtherObj2->SetPos(CurPos.x, CurPos.y -= (ScaleY - vDiff.y));

	}

}


void CPlatform::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pOtherObj = dynamic_cast<CPlayer*>(_OtherObj);
	CMonster* pOtherObj2 = dynamic_cast<CMonster*>(_OtherObj);

	if (pOtherObj)
	{
		CRigidBody* pRigidBody = pOtherObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pOtherObj2)
	{
		CRigidBody* pRigidBody2 = pOtherObj2->GetComponent<CRigidBody>();
		pRigidBody2->SetGround(false);
	}

	
}

