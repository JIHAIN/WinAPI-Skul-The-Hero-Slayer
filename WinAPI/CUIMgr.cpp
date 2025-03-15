#include "pch.h"
#include "CUIMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CUI.h"
#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_PreventFrame(0)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::Tick()
{
	if (m_PreventFrame)
	{
		--m_PreventFrame;
		return;
	}
	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	//현재 레벨을 확인
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();

	// 현재 벡터에 담긴 모든 UI 레이어를 가져온다.

	// 원래는  수정을 안하려고 const & 로 받아왔는데 이젠 수정이 필요해서 const를 없앤다.
	//const vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER_TYPE::UI);
	vector<CObj*>& vecUI = pCurLevel->GetLayer(LAYER_TYPE::UI);

	vector<CObj*>::reverse_iterator iter = vecUI.rbegin();

	// 리버스 이터를 사용해서 뒤에서 부터 앞으로 순차적으로 검사해간다.
	// 여기서 걸린 얘가 포커싱을 받아야 하니까 벡터의 맨 뒤로 보내고 랜더도 맨 뒤로 보내줌.
	// 그리고 더이상 반복문을 돌 필요도 없어짐
	for (; iter !=  vecUI.rend(); ++iter)
	{
		// 혹시나를 위해 예외처리
		CUI* pUI = dynamic_cast<CUI*>((*iter));
		assert(pUI);

		// 부모 UI를 입력해서 질제 이벤트를 가져갈 우선순위 UI를 알아냄
		// 본인이 나올 수도 있고 자식이 나올 수도 있다.
		CUI* pPriorityUI = GetPriorityUI(pUI);

		if (nullptr == pPriorityUI) // 마우스가 위에 올라온 UI가 없으면 다음 최상위 UI로 넘어간다.
		{
			StatusCheck(pUI); // 상태 체크 직전 프레임중에 눌림으로 된 놈들을 눌림 해제를 해줌
			continue;
		}
			

		// pPriorityUI 는 마우스 이벤트를 가져가야할 UI를 말하는 거임
		
		if (LBtnState == KEY_STATE::TAP) // 눌린상태
		{
			pPriorityUI->m_LbtnDown = true;
			pPriorityUI->LBtnDown();

			// 벡터에서 뺀 다음 푸쉬백으로 맨 뒤에 집어넣는다.
			vecUI.erase((iter + 1).base());
			vecUI.push_back(pUI);

			break;
		}

		if (LBtnState == KEY_STATE::RELEASED)
		{
			//pUI->m_LbtnDown = false; 이걸 여기서 호출하면 꼭 UI위에서만 떼져야지 체크가 됨으로 여기서 체크안함
			
			pPriorityUI->LBtnUP(); // UI위에서 만 UP이 실행됌
			if (pPriorityUI->m_LbtnDown) // 해당 UI 위에서 눌렸고 지금 떼진 상태라면 클릭판정
			{
				pPriorityUI->LBtnClicked();
			}
			StatusCheck(pUI);
		}

		StatusCheck(pUI); // 상태체크 모든 키 순회 돌면서 LBtnDown을 false로 만들어줌
		break;
	}
	
}

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI)
{
	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// 반환 값으로 줄 우선순위ui 포인터 
	CUI* pPriorityUI = nullptr; 

	// 레벨순회
	static list<CUI*> queue;
	queue.clear(); // 정적변수 선언을 해둬서 계속 데이터가 차있음으로 매번 클리어를 해준다.

	// 최상위 부모 UI입력
	queue.push_back(_ParentUI); 

	while (!queue.empty()) // 큐가 비어있지 않을때
	{
		CUI* pUI = queue.front(); // 앞에 있는걸 꺼내옴
		queue.pop_front(); // 앞을 비워줌


		// 내 위에 마우스가 있는지 없는지 체크
		if (pUI->m_MouseOn) // 이미 개별 UI들 다 Tick에서 체크 해뒀음
		{
			pPriorityUI = pUI; // 가장 마지막 자식중 마우스가 위에 있는 UI가 이벤트를 가져가야한다.
		}

		

		// 마우스 On 관련 이벤트
		if (false == pUI->m_MouseOn_Prev && pUI->m_MouseOn) // 이번 프레임에 처음 마우스가 올라왔을 시
			pUI->BeginHovered();
		else if (pUI->m_MouseOn_Prev && pUI->m_MouseOn) // 마우스가 계속 위에있음.
			pUI->OnHovered();
		else if (pUI->m_MouseOn_Prev && !pUI->m_MouseOn) // 마우스 벗어남.
			pUI->EndHovered();


		const vector<CUI*>& vecChild = pUI->GetChildUI(); // 입력으로 들어온 UI의 자식이 있으면 받음.
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}
	// UI에 있는 모든 자식관계를 다 순회해서 찾아냄 
	return pPriorityUI;
}

void CUIMgr::StatusCheck(CUI* _ParentUI)
{
	// 마우스 왼쪽버튼의 상태를 체크한다.
	KEY_STATE LBtnState = CKeyMgr::Get()->GetKeyState(KEY::LBTN);

	// 레벨순회
	static list<CUI*> queue;
	queue.clear(); // 정적변수 선언을 해둬서 계속 데이터가 차있음으로 매번 클리어를 해준다.

	// 최상위 부모 UI입력
	queue.push_back(_ParentUI);

	while (!queue.empty()) // 큐가 비어있지 않을때
	{
		CUI* pUI = queue.front(); // 앞에 있는걸 꺼내옴
		queue.pop_front(); // 앞을 비워줌


		const vector<CUI*>& vecChild = pUI->GetChildUI(); // 입력으로 들어온 UI의 자식이 있으면 받음.
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}


		// 왼쪽 버튼 떼진 상태 UI밖에서 어디든지 상관없음, 모든 UI들은 눌림상태를 false로 바꿈
		if (LBtnState == KEY_STATE::RELEASED)
		{
			pUI->m_LbtnDown = false;
		}
	}
	// UI에 있는 모든 자식관계를 다 순회해서 찾아냄 
}
