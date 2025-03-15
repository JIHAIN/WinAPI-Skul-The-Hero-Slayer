#pragma once
#include "CUI.h"

typedef void(*BTN_CALLBACK)(void);    // Ÿ�� ������ ���Ƿ� ��ȯŸ���� ���̵忡 ���ڵ� ���̵��� �ڷ����� �����.
typedef void(CBase::* BaseFunc)(void);// ���̽��� �Ļ��� �ɹ��Լ� ���� ��ȯŸ���� ���̵忡 ���ڵ� 
                                      // ���̵��� ����Լ� �����ͺ���

// �̰� ��ȯ ���̵� ���� ���̵常 ���� �� �־ Ư���Լ� �̿��ҰŸ� ���ڸ� �׿� ���缭 �غ�������Ѵ�.

// ���� DWORD_PTR�� �޴� Ÿ�� �ϳ� ����� 8����Ʈ�� ���ڵ� �ּҵ� ���� �� ����
typedef void(CBase::* BaseFunc_Param)(DWORD_PTR);

class CBtnUI :
    public CUI
{
private:
    BTN_CALLBACK   m_CallBack; // ���� �������� �̸��� �Լ� ������ ������ �����.

    CBase*         m_Inst;     // �Լ� �������� ����Լ� ����� ���� ��� Ŭ������ ����ų�� �ִ� CBase �����͸� �޾Ƶд�
    BaseFunc       m_MemFunc;
    BaseFunc_Param m_MemFunc_1; // �̰� ���� DWORD_PTR �� �ϳ� �޴� �����̴�.



public:
    // �����Լ��� ����ϴ� �ݹ� �Լ��� ����
    void SetCallBack(BTN_CALLBACK _CallBack) { m_CallBack = _CallBack; }

    // �ɹ��Լ��� ����Ѵ� ��������Ʈ ��ü�� �Լ��� ����
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
    CLONE(CBtnUI); // �Լ� �������� �ּҸ� �ٲٴ°� ���� ������ �ƴϱ⿡ �׳� �״�� ���� ��
    CBtnUI();
    ~CBtnUI();
};

// ��ư�� ������ �Լ��� �����ع����� �� �� �ۿ� ���ϴϱ� ��ư�� �ؾ��� ���� ������ ����
// ��ü���� �ٸ� ���� �� �� �ְ� �����.
// �Լ� �����͸� �̿��ؼ� ��ư�� �����ؾ��� ������ �������ش�.

// void SetCAllBack(BTN_CALLBACK _CallBack) �Լ� ����
// %%�������� �����Ҷ� ������ �� �������%% 
// ���ϴ� �Լ��� �����Լ��� �����Ŀ� ���⿡ ���ڷ� &�Լ��̸� �־��ָ� �� ��
// �׸��� ������ ��ư ��ü���� ex) pUI->SetCallBack(&�Լ��̸�) ���� ������ ���ش� �׷� Ŭ���� ������ �� �Լ��� �����.

// �Լ������ͷ� ����Լ����� ����ϱ� ���ؼ��� �� �� ��ü�� �ʿ��ϱ� ������
// ��ü���� �ѹ��� �޾ƿͼ� ����Լ��� ����Ҽ� �ְ� �����.

// void SetDelegate ����
// SetDelegate(����� ��ü, (BaseFund)&����Ұ�ü::������Լ�())
// ���������� �ȿ��� ���������� �Լ� ����ϴ� ����
// pUI->SetDelegate(this, (BaseFund)&����Ұ�ü::������Լ�())

// �� ���� ������ �Լ� ��������Ʈ ���� C++�������� �ϴ°Ű� �̰� ���ϰ� �ϴ� ��ųʸ� ��°� �մܴ� �˻��غ���.

