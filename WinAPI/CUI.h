#pragma once
#include "CObj.h"


class CUI :
    public CObj
{
private:
    CUI*          m_Parent;    // �θ� ����Ű�� ������ 
    vector<CUI*>  m_vecChild; // �ڽĵ��� ����Ű������ ������ �ִ� ����

    Vec2          m_FinalPos;  

    bool    m_MouseOn;         // ���� ���콺�� UI ���� �ִ���
    bool    m_MouseOn_Prev;    // ���� �����ӿ� ���콺�� UI���� �־�����
    bool    m_LbtnDown;        // UI�� ���콺 ���� ��ư�� ���� ��������.

public:
    CUI* GetParentUI() { return m_Parent; } // �θ� ��ȯ�ϴ� �Լ� ���Ͻ� ���� �ֻ��� UI
    const vector<CUI*>& GetChildUI() { return m_vecChild; } // �ڽ��� ��ȯ�ϴ� �Լ�
    void AddChildUI(CUI* _ChildUI)   // �ڽ��� �߰��ϴ� �Լ� �ڽ��� ���Ϳ� ��������鼭 �θ� �������� �������ش�.
    {
        m_vecChild.push_back(_ChildUI);
        _ChildUI->m_Parent = this;
    }
    // �ֵ� ���ϵ� ���� �θ� UI (���� �г�) ��ü ����� �� ��ü�� ->AddChildUI() �� �߰�����

    Vec2 GetFinalPos() { return m_FinalPos; }
    bool IsLBtnDown() { return m_LbtnDown; }
    bool IsMouseOn() { return m_MouseOn; }

public:
    virtual void Tick() override final; // final Ű���尡 ������ �� ������ ���̻� �������̵� �� ���� ��������.
    virtual void FinalTick() override;
    virtual void Render() override final;

    virtual void Tick_UI() = 0;     // UI �鸸 ����ϴ� �ڽĵ��� ����ϱ� ���� ƽ�� ����
    virtual void Render_UI();

    // ���콺 ������ �ִϸ��̼� �ִ� ��Ȳ�� ��밡�� �ƴϸ� �÷��̾� ��ġ �޾Ƽ� ��ó�� ���� Ű Ȱ��ȭ �̺�Ʈ �����ҵ�
    virtual void BeginHovered() {} /// ó�� ���콺�ö������
    virtual void OnHovered() {} /// ���콺�� �ö�� �ִ� ����
    virtual void EndHovered() {} /// ���콺�� ���� ���� ��

    virtual void LBtnDown() {}
    virtual void LBtnUP() {}
    virtual void LBtnClicked() {}

private:
    virtual void MouseOnCheak(); // �����Լ��� ������ ������ HP�� ó�������̴� UI�� ������ǥ�� ���ؾ߱� ������ ����

public:
    virtual CUI* Clone() override = 0; // ���������Լ��� ���� �ڽ��� �����ϵ��� ��.
    CUI();
    CUI(const CUI& _Other); // �ڽĵ��� ���ؼ� ��������� ���� ����� ������
    ~CUI();

    friend class CUIMgr;
};

// ������ ���������� �ö󰡴� UI�� �θ� ���� �ֻ��� UI�� �ö� �� �ִ�.
// ��ġ ������ ���� �θ�ȿ� �ڽ��� �ִ°ű� ������ �θ��� ��ġ�� Ȯ�� �ǰ� �ڽ��� �װ� �޾Ƽ� �ڱ� ��ġ��
// �����ؾ� ������ ������ �θ� UI���Ը� ƽ�� �ְ� �θ� ��ü������ �ڱⰡ ����ִ� �ڽĵ鿡�� ƽ�� �Ѱ��ش�.