
#pragma once
#include "CMissile.h"

class CAnimator;

class CKiMissile :
	public CMissile
{
private:
	CObj*				m_TargetObj;
	CAnimator*			m_Animator;

	float               m_RotateSpeed;
	float               m_AccTime;

public:
	virtual void Tick() override;
	virtual void Render() override;

private:
	void FindTarget();
	void Trace();
	virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
public:
	CLONE(CKiMissile);
	CKiMissile();
	CKiMissile(const CKiMissile& _Other);
	~CKiMissile();
};