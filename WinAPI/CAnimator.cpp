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
	//���ڰ� const Ÿ���̶� ���ͷ����͵� const_iterator �� ����ؾ���
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
	// �̹� ���� �̸��� Animation �� �ִ��� Ȯ��
	CAnim* pAnim = FindAnimation(_Info.AnimName);
	assert(nullptr == pAnim);

	// Animation �ϳ� �����ϰ� �Էµ� ������ �˷���
	pAnim = new CAnim;
	pAnim->Create(_Info);

	// Animator �� ������Ų Animation ���
	pAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
}

void CAnimator::CreateAnimationEdit(const tAnimDesc& _Info)
{
	// �̹� ���� �̸��� Animation �� �ִ��� Ȯ��
	CAnim* pAnim = FindAnimation(_Info.AnimName);

	//ã�Ҵµ� ���� �̸��� �����ÿ� �ű⿡ ���� �߰�
	if (pAnim != nullptr)
	{
		pAnim->CreateEditPlus(_Info);

		// Animator �� ������Ų Animation ���
		pAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
	}
	// ������ �� ���ο� ���� �߰�
	else if (pAnim == nullptr)
	{
		// Animation �ϳ� �����ϰ� �Էµ� ������ �˷���
		pAnim = new CAnim;
		pAnim->Create(_Info);

		// Animator �� ������Ų Animation ���
		pAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
	}

}

void CAnimator::AddFrameToAnimation(const wstring& _animName, const tAnimFrm& _frame)
{
	CAnim* pAnim = FindAnimation(_animName);

	//if (pAnim != nullptr) ���� �ƴϸ� �߰��Ϸ� �ߴµ� �����غ��� ����� ����������
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
		// �޸� ����
		Release_Map(m_mapAnim);


	}
}


void CAnimator::Play(const wstring& _Name, bool _Repeat)
{
	// m_CurAnim = FindAnimation(_Name);
	// m_Repeat = _Repeat;
	// 
	// // ���¸ӽſ��� �ִϸ��̼� �÷����ϸ� �̰� Ȱ��ȭ�ص� ��
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
			m_CurAnim->Play();   // �ִϸ��̼� ��� ����
		}
	}




}

void CAnimator::Render()
{
	// ������� Animation �� ������ ��ȯ
	if (!m_CurAnim)
		return;

	m_CurAnim->Render();
}

void CAnimator::UI_Render()
{
	// ������� Animation �� ������ ��ȯ
	if (!m_CurAnim)
		return;

	m_CurAnim->UI_Render();
}



void CAnimator::FinalTick()
{
	//// ������� Animation �� ������ ��ȯ
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

		// ��ȯ �ʿ� ������ ���� �ִϸ��̼��� �ִ��� Ȯ��
		if (m_TransitionMap.find(m_CurAnim->GetName()) != m_TransitionMap.end())
		{
			nextAnimName = m_TransitionMap[m_CurAnim->GetName()];
		}
		// ���� �ִϸ��̼����� ���ư��� �ϴ��� Ȯ��
		else if (m_ReturnToPrevious && m_PrevAnim)
		{
			nextAnimName = m_PrevAnim->GetName();
			m_PrevAnim = nullptr;
			m_ReturnToPrevious = false;
		}
		// �ݺ� ���
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

// ���� �ִϸ��̼ǰ� ���� �ִϸ��̼��� ����
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

// �ִϸ��̼� ���� ������Ʈ �� ��ȯ
void CAnimator::UpdateAnimationState()
{
	if (!m_CurAnim)
		return;

	m_CurAnim->FinalTick();  // ���� �ִϸ��̼� ������Ʈ

	// ���� �ִϸ��̼��� �����ų� ��� ���� �ƴϸ�
	if (m_CurAnim->IsFinish() || !m_CurAnim->IsPlaying())
	{
		if (m_NextAnim)
		{
			// ���� �ִϸ��̼����� ��ȯ
			Play(m_NextAnim->GetName(), m_Repeat);
			m_NextAnim = nullptr;
		}
		else if (m_Repeat)
		{
			// �ݺ� ��� �����̸� ���� �ִϸ��̼� �����
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
