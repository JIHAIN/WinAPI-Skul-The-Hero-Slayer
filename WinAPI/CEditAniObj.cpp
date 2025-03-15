#include "pch.h"
#include "CEditAniObj.h"
#include "CAnimator.h"

CEditAniObj::CEditAniObj()
	: m_Animator(nullptr)
{

	m_Animator = AddComponent(new CAnimator);

	//m_Animator->LoadAnimation(L"animation\\Player\\IDLE_Right.anim");
	//m_Animator->LoadAnimation(L"animation\\Player\\IDLE_Left.anim");
	//
    //m_Animator->Play(L"IDLE_Right", true);

}

CEditAniObj::CEditAniObj(const CEditAniObj& _Other)
	: CObj(_Other)
	, m_Animator(nullptr)
{
	m_Animator = GetComponent<CAnimator>();
}

CEditAniObj::~CEditAniObj()
{
}

void CEditAniObj::Render()
{
	if (m_Animator != nullptr)
	{
		m_Animator->Render();
	}
}