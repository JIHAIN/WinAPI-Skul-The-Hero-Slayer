#include "pch.h"
#include "CAnimator.h"

#include "CPathMgr.h"
#include "CAnim.h"

CAnimator::CAnimator()
	: CComponent(COMPONENT_TYPE::ANIMATOR)
	, m_CurAnim(nullptr)
	, m_Repeat(false)
{

}

CAnimator::CAnimator(const CAnimator& _Other)
	: CComponent(_Other)
	, m_CurAnim(nullptr)
	, m_Repeat(_Other.m_Repeat)
{
	//인자가 const 타입이라 이터레이터도 const_iterator 을 사용해야함
	map<wstring, CAnim*>::const_iterator iter = _Other.m_mapAnim.begin();
	for (; iter != _Other.m_mapAnim.end(); ++iter)
	{
		CAnim* pCloneAnim = iter->second->Clone();

		pCloneAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
	}


	if (_Other.m_CurAnim)
	{
		m_CurAnim = FindAnimation(_Other.m_CurAnim->GetName());
	}
}

CAnimator::~CAnimator()
{
	Release_Map(m_mapAnim);
}

void CAnimator::CreateAnimation(const tAnimDesc& _Info)
{
	// 이미 같은 이름의 Animation 이 있는지 확인
	CAnim* pAnim = FindAnimation(_Info.AnimName);
	assert(nullptr == pAnim);

	// Animation 하나 생성하고 입력된 정보를 알려줌
	pAnim = new CAnim;
	pAnim->Create(_Info);

	// Animator 에 생성시킨 Animation 등록
	pAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
}

void CAnimator::CreateAnimationEdit(const tAnimDesc& _Info)
{
	// 이미 같은 이름의 Animation 이 있는지 확인
	CAnim* pAnim = FindAnimation(_Info.AnimName);

	//찾았는데 같은 이름이 있을시에 거기에 정보 추가
	if (pAnim != nullptr)
	{
		pAnim->CreateEditPlus(_Info);

		// Animator 에 생성시킨 Animation 등록
		pAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
	}
	// 없었을 시 새로운 정보 추가
	else if (pAnim == nullptr)
	{
		// Animation 하나 생성하고 입력된 정보를 알려줌
		pAnim = new CAnim;
		pAnim->Create(_Info);

		// Animator 에 생성시킨 Animation 등록
		pAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
	}

}

void CAnimator::AddFrameToAnimation(const wstring& _animName, const tAnimFrm& _frame)
{
	CAnim* pAnim = FindAnimation(_animName);

	//if (pAnim != nullptr) 널이 아니면 추가하려 했는데 생각해보니 없었어도 만들어줘야함
	//{
	//	pAnim->AddFrame(_frame);
	//}

	pAnim->AddFrame(_frame);
}

CAnim* CAnimator::FindAnimation(const wstring& _Name)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_Name);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator::DeleteAnimation(wstring _Name)
{
	auto iter = m_mapAnim.find(_Name);

	if (iter != m_mapAnim.end())
	{
		// 메모리 해제
		Release_Map(m_mapAnim);


	}
}


void CAnimator::Play(const wstring& _Name, bool _Repeat)
{
	// m_CurAnim = FindAnimation(_Name);
	// m_Repeat = _Repeat;
	// 
	// // 상태머신에서 애니메이션 플레이하면 이거 활성화해도 됌
	// //m_CurAnim->Reset();
	// 
	// if (m_CurAnim)
	// {
	// 	if (m_CurAnim->IsFinish())
	// 	{
	// 		m_CurAnim->Reset();
	// 	}
	// }

	if (m_CurAnim == FindAnimation(_Name))
	{
		return;
	}

	else
	{
		m_CurAnim = FindAnimation(_Name);
		m_Repeat = _Repeat;

		if (m_CurAnim)
		{
			m_CurAnim->Reset();
			m_CurAnim->Play();   // 애니메이션 재생 시작
		}
	}




}

void CAnimator::Render()
{
	// 재생중인 Animation 이 없으면 반환
	if (!m_CurAnim)
		return;

	m_CurAnim->Render();
}

void CAnimator::UI_Render()
{
	// 재생중인 Animation 이 없으면 반환
	if (!m_CurAnim)
		return;

	m_CurAnim->UI_Render();
}



void CAnimator::FinalTick()
{
	//// 재생중인 Animation 이 없으면 반환
	//if (!m_CurAnim)
	//	return;
	//
	//if (m_Repeat && m_CurAnim->IsFinish())
	//{
	//	m_CurAnim->Reset();
	//}
	//
	//m_CurAnim->FinalTick();


	if (!m_CurAnim)
		return;

	m_CurAnim->FinalTick();

	if (m_CurAnim->IsFinish())
	{
		wstring nextAnimName;

		// 전환 맵에 설정된 다음 애니메이션이 있는지 확인
		if (m_TransitionMap.find(m_CurAnim->GetName()) != m_TransitionMap.end())
		{
			nextAnimName = m_TransitionMap[m_CurAnim->GetName()];
		}
		// 이전 애니메이션으로 돌아가야 하는지 확인
		else if (m_ReturnToPrevious && m_PrevAnim)
		{
			nextAnimName = m_PrevAnim->GetName();
			m_PrevAnim = nullptr;
			m_ReturnToPrevious = false;
		}
		// 반복 재생
		else if (m_Repeat)
		{
			m_CurAnim->Reset();
			return;
		}

		if (!nextAnimName.empty())
		{
			Play(nextAnimName, m_Repeat);
		}
	}


}



void CAnimator::SaveAnimation(const wstring& _RelativeFolder)
{
	wstring strFolderPath = CPathMgr::Get()->GetContentPath();
	strFolderPath += _RelativeFolder;

	map<wstring, CAnim*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->Save(strFolderPath);
	}
}

void CAnimator::SaveEditAnimation(const wstring& _RelativeFolder, wstring _Name)
{
	wstring strFolderPath = CPathMgr::Get()->GetContentPath();
	strFolderPath += _RelativeFolder;

	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_Name);

	if (iter == m_mapAnim.end())
	{
		return;
	}
	else
	{
		iter->second->Save(strFolderPath);
	}
}


void CAnimator::LoadAnimation(const wstring& _RelativePath)
{
	wstring strFilePath = CPathMgr::Get()->GetContentPath();
	strFilePath += _RelativePath;

	CAnim* pNewAnim = new CAnim;
	pNewAnim->Load(strFilePath);

	assert(!FindAnimation(pNewAnim->GetName()));

	if (FindAnimation(pNewAnim->GetName()) != nullptr)
	{
		return;
	}

	pNewAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
}

// 현재 애니메이션과 다음 애니메이션을 설정
void CAnimator::SetNextAnimation(const wstring& _CurName, const wstring& _NextName)
{
	CAnim* pCurAnim = FindAnimation(_CurName);
	CAnim* pNextAnim = FindAnimation(_NextName);

	if (pCurAnim && pNextAnim)
	{
		m_CurAnim = pCurAnim;
		m_NextAnim = pNextAnim;
	}
}

// 애니메이션 상태 업데이트 및 전환
void CAnimator::UpdateAnimationState()
{
	if (!m_CurAnim)
		return;

	m_CurAnim->FinalTick();  // 현재 애니메이션 업데이트

	// 현재 애니메이션이 끝났거나 재생 중이 아니면
	if (m_CurAnim->IsFinish() || !m_CurAnim->IsPlaying())
	{
		if (m_NextAnim)
		{
			// 다음 애니메이션으로 전환
			Play(m_NextAnim->GetName(), m_Repeat);
			m_NextAnim = nullptr;
		}
		else if (m_Repeat)
		{
			// 반복 재생 설정이면 현재 애니메이션 재시작
			m_CurAnim->Reset();
		}
	}
}


void CAnimator::InterruptAnimation(const wstring& _Name, bool _ReturnToPrevious)
{
	if (m_CurAnim)
	{
		m_PrevAnim = m_CurAnim;
	}
	Play(_Name, false);
	m_ReturnToPrevious = _ReturnToPrevious;
}


void CAnimator::ResumeAnimation()
{
	if (m_PrevAnim)
	{
		Play(m_PrevAnim->GetName(), m_Repeat);
		m_PrevAnim = nullptr;
	}
}
