#pragma once
#include "CObj.h"


class CUI :
    public CObj
{
private:
    CUI*          m_Parent;    // 부모를 가르키는 포인터 
    vector<CUI*>  m_vecChild; // 자식들을 가르키기위해 가지고 있는 벡터

    Vec2          m_FinalPos;  

    bool    m_MouseOn;         // 현재 마우스가 UI 위에 있는지
    bool    m_MouseOn_Prev;    // 이전 프레임에 마우스가 UI위에 있었는지
    bool    m_LbtnDown;        // UI에 마우스 왼쪽 버튼이 눌린 상태인지.

public:
    CUI* GetParentUI() { return m_Parent; } // 부모를 반환하는 함수 널일시 가장 최상위 UI
    const vector<CUI*>& GetChildUI() { return m_vecChild; } // 자식을 반환하는 함수
    void AddChildUI(CUI* _ChildUI)   // 자식을 추가하는 함수 자식을 벡터에 집어넣으면서 부모를 본인으로 설정해준다.
    {
        m_vecChild.push_back(_ChildUI);
        _ChildUI->m_Parent = this;
    }
    // 애드 차일드 사용법 부모 UI (보통 패널) 객체 만들고 그 객체에 ->AddChildUI() 로 추가해줌

    Vec2 GetFinalPos() { return m_FinalPos; }
    bool IsLBtnDown() { return m_LbtnDown; }
    bool IsMouseOn() { return m_MouseOn; }

public:
    virtual void Tick() override final; // final 키워드가 붙으면 이 밑으로 더이상 오버라이딩 할 수가 없어진다.
    virtual void FinalTick() override;
    virtual void Render() override final;

    virtual void Tick_UI() = 0;     // UI 들만 사용하는 자식들이 사용하기 위한 틱과 랜더
    virtual void Render_UI();

    // 마우스 감지에 애니메이션 넣는 상황에 사용가능 아니면 플레이어 위치 받아서 근처에 오면 키 활성화 이벤트 가능할듯
    virtual void BeginHovered() {} /// 처음 마우스올라왔을때
    virtual void OnHovered() {} /// 마우스가 올라와 있는 상태
    virtual void EndHovered() {} /// 마우스가 위에 없을 때

    virtual void LBtnDown() {}
    virtual void LBtnUP() {}
    virtual void LBtnClicked() {}

private:
    virtual void MouseOnCheak(); // 가상함수로 지정한 이유는 HP바 처럼움직이는 UI는 랜더좌표로 비교해야기 때문에 설정

public:
    virtual CUI* Clone() override = 0; // 순수가상함수를 위해 자식이 구현하도록 함.
    CUI();
    CUI(const CUI& _Other); // 자식들을 위해서 복사생성자 깊은 복사로 구현함
    ~CUI();

    friend class CUIMgr;
};

// 레벨에 직접적으로 올라가는 UI는 부모가 없는 최상위 UI만 올라갈 수 있다.
// 위치 설정만 봐도 부모안에 자식이 있는거기 떄문에 부모의 위치가 확정 되고 자식이 그걸 받아서 자기 위치를
// 지정해야 함으로 레벨은 부모 UI에게만 틱을 주고 부모가 자체적으로 자기가 들고있는 자식들에게 틱을 넘겨준다.