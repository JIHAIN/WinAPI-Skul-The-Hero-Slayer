#pragma once
#include "CUI.h"

typedef void(*BTN_CALLBACK)(void);    // 타입 디파인 정의로 반환타입이 보이드에 인자도 보이드인 자료형을 만든다.
typedef void(CBase::* BaseFunc)(void);// 베이스로 파생된 맴버함수 선언 반환타입은 보이드에 인자도 
                                      // 보이드인 멤버함수 포인터변수

// 이건 반환 보이드 인자 보이드만 받을 수 있어서 특정함수 이용할거면 인자를 그에 맞춰서 준비해줘야한다.

// 인자 DWORD_PTR를 받는 타입 하나 만들기 8바이트라 숫자도 주소도 받을 수 있음
typedef void(CBase::* BaseFunc_Param)(DWORD_PTR);

class CBtnUI :
    public CUI
{
private:
    BTN_CALLBACK   m_CallBack; // 위에 재정의한 이름의 함수 포인터 변수를 만든다.

    CBase*         m_Inst;     // 함수 포인터의 멤버함수 사용을 위해 모든 클래스를 가르킬수 있는 CBase 포인터를 받아둔다
    BaseFunc       m_MemFunc;
    BaseFunc_Param m_MemFunc_1; // 이건 인자 DWORD_PTR 를 하나 받는 버전이다.



public:
    // 전역함수를 사용하는 콜백 함수만 받음
    void SetCallBack(BTN_CALLBACK _CallBack) { m_CallBack = _CallBack; }

    // 맴버함수를 사용한느 델리게이트 객체와 함수를 받음
    void SetDelegate(CBase* _Inst, BaseFunc _MemFunc)
    {
        m_Inst = _Inst;
        m_MemFunc = _MemFunc;
    }

    virtual void Tick_UI() override {};
    virtual void Render_UI() override;

public:
    virtual void LBtnDown() override;
    virtual void LBtnClicked() override;


public:
    CLONE(CBtnUI); // 함수 포인터의 주소를 바꾸는건 복사 개념이 아니기에 그냥 그대로 쓰면 댐
    CBtnUI();
    ~CBtnUI();
};

// 버튼의 할일을 함수로 지정해버리면 그 일 밖에 못하니까 버튼별 해야할 일은 변수로 만들어서
// 객체별로 다른 일을 할 수 있게 만든다.
// 함수 포인터를 이용해서 버튼별 수행해야할 동작을 지정해준다.

// void SetCAllBack(BTN_CALLBACK _CallBack) 함수 사용법
// %%레벨에서 생성할때 세팅을 다 해줘야함%% 
// 원하는 함수를 전역함수로 만든후에 저기에 인자로 &함수이름 넣어주면 됌 끝
// 그리고 생성된 버튼 객체에게 ex) pUI->SetCallBack(&함수이름) 으로 세팅을 해준다 그럼 클릭시 했을때 그 함수를 사용함.

// 함수포인터로 멤버함수까지 사용하기 위해서는 꼭 그 객체가 필요하기 때문에
// 객체까지 한번에 받아와서 멤버함수를 사용할수 있게 만든다.

// void SetDelegate 사용법
// SetDelegate(사용할 객체, (BaseFund)&사용할객체::사용할함수())
// 레벨에디터 안에서 레벨에디터 함수 사용하는 예제
// pUI->SetDelegate(this, (BaseFund)&사용할객체::사용할함수())

// 저 위에 포인터 함수 델리게이트 등은 C++문법으로 하는거고 이걸 편하게 하는 펑셔너리 라는게 잇단다 검색해보자.

