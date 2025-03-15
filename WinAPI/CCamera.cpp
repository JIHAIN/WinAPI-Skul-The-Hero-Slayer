#include "pch.h"
#include "CCamera.h"

#include "CEngine.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CLevelMgr.h"
#include "CLevel_Editor.h"


CCamera::CCamera()
	: m_CamSpeed(2000.f)
	, m_LookAt(0.f, 0.f)
	, m_LookAtDiff(0.f, 0.f)
	, m_ShakeOffset(0.f, 0.f)
	, m_TargetPos(0.f, 0.f)
	, m_MapMin(0.f, 0.f)
	, m_MapMax(0.f, 0.f)
	, m_LerpSpeedX(5.0f)
	, m_LerpSpeedY(2.0f)
	, m_DefaultPlayerY(0.6f)  // ȭ�� ������ 60% ��ġ�� �ʱ�ȭ
	, m_CameraAdjustThreshold(0.1f)  // 10%�� ������ ����
	, m_CameraAdjustSpeed(0.5f)  // ī�޶� ���� �ӵ�
	, m_BlackFilterTex(nullptr)
	, m_Frequency(0.f)
	, m_Amplitude(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
	, m_Dir(0.f)
	, m_CamShake(false)
{
	// Black Filter Texture ����
	Vec2 vResolution = CEngine::Get()->GetResolution();
	m_BlackFilterTex = CAssetMgr::Get()->CreateTexture(L"BlackFliterTex", (UINT)vResolution.x, (UINT)vResolution.y);

	m_FilterInfo.Effect = NONE;
}

CCamera::~CCamera()
{
}

Vec2 CCamera::PlayerMoveLimit(const Vec2& position)
{
	// ī�޶� �߽� ��ġ
	Vec2 cameraCenter = m_LookAt;

	// ȭ�� ũ���� ����
	Vec2 halfScreenSize = CEngine::Get()->GetResolution() / 2.f;

	// �÷��̾� �ݶ��̴��� ũ�� (���� ��, ���� ũ��� ���� �ʿ�)
	Vec2 playerColliderSize(50.f, 60.f);

	// �÷��̾ ȭ�� ���� ������ �������� ���� ���� ����
	Vec2 padding = playerColliderSize /2;

	// ī�޶� ���� ��� (�÷��̾� �ݶ��̴� ���)
	float minX = cameraCenter.x - halfScreenSize.x + padding.x;
	float maxX = cameraCenter.x + halfScreenSize.x - padding.x;
	float minY = cameraCenter.y - halfScreenSize.y + padding.y;
	//float maxY = cameraCenter.y + halfScreenSize.y - padding.y;
	float maxY = 1668;
	return Vec2(Clamp(position.x, minX, maxX),Clamp(position.y, minY, maxY));
}

void CCamera::Init()
{
	Vec2 vResolution = CEngine::Get()->GetResolution();
	m_LookAt = vResolution / 2.f;
}

void CCamera::Tick()
{
	//���������� �޾ƿ���
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
	// ���� ������ ������ �ƴѰ����� �޴��ٰ� ȣ�� �ƴٸ� ���� ������  �� ����Ʈ�� ��������״� ���Ʈ�� �Ǵ�.

	if (pEditorLevel) // ���� ������ �ϋ�
	{
		if (KEY_PRESSED(KEY::W))
			m_LookAt.y -= m_CamSpeed * DT;

		if (KEY_PRESSED(KEY::S))
			m_LookAt.y += m_CamSpeed * DT;

		if (KEY_PRESSED(KEY::A))
			m_LookAt.x -= m_CamSpeed * DT;

		if (KEY_PRESSED(KEY::D))
			m_LookAt.x += m_CamSpeed * DT;
	}

	else // �ƴҋ�
	{
		// ȭ�� �ػ� �� ȭ�� �߾� ��ġ ���
		Vec2 resolution = CEngine::Get()->GetResolution();
		Vec2 halfScreenSize = resolution / 2.f;

		// x��: Ÿ�� ��ġ�� �ε巴�� �̵�
		float newLookAtX = Lerp(m_LookAt.x, m_TargetPos.x, m_LerpSpeedX * DT);

		// y��: �÷��̾��� ���̿� ���� �ε巴�� ����
		float playerScreenY = m_TargetPos.y - (m_LookAt.y - halfScreenSize.y);
		float normalizedPlayerY = playerScreenY / resolution.y;

		// ��ǥ Y ��ġ ���
		float targetY = m_LookAt.y; // �⺻������ ���� y ��ġ ����

		// �ε巯�� ī�޶� �̵��� ���� ������ ����
		float upperDeadzone = m_DefaultPlayerY - m_CameraAdjustThreshold;
		float lowerDeadzone = m_DefaultPlayerY + m_CameraAdjustThreshold;

		if (normalizedPlayerY < upperDeadzone)
		{
			// �÷��̾ ��� �������� ����� �� �ε巴�� ���� �̵�
			float diff = upperDeadzone - normalizedPlayerY;
			targetY = m_LookAt.y - (diff * resolution.y * m_CameraAdjustSpeed);
		}
		else if (normalizedPlayerY > lowerDeadzone)
		{
			// �÷��̾ �ϴ� �������� ����� �� �ε巴�� �Ʒ��� �̵�
			float diff = normalizedPlayerY - lowerDeadzone;
			targetY = m_LookAt.y + (diff * resolution.y * m_CameraAdjustSpeed);
		}

		// �ε巯�� Y�� �̵� ����
		float newLookAtY = Lerp(m_LookAt.y, targetY, m_LerpSpeedY * DT);

		// ī�޶� ��ġ�� �� ���� ���� ����
		newLookAtX = Clamp(newLookAtX, m_MapMin.x + halfScreenSize.x, m_MapMax.x - halfScreenSize.x);
		newLookAtY = Clamp(newLookAtY, m_MapMin.y + halfScreenSize.y, m_MapMax.y - halfScreenSize.y);

		// ���ο� ī�޶� ��ġ ����
		m_LookAt = Vec2(newLookAtX, newLookAtY);
		// ī�޶� ȿ�� ����
		CameraEffect();
	}


	// ī�޶� �����ִ� ������, �ػ� �߽���ġ�� ���̰�
	m_LookAtDiff = (m_LookAt + m_ShakeOffset) - (CEngine::Get()->GetResolution() / 2.f);
}

void CCamera::Render()
{
	if (m_FilterInfo.Effect == NONE)
		return;

	UINT Alpha = 0;

	if (FILTER_EFFECT::FADE_OUT == m_FilterInfo.Effect)
	{
		float fRatio = m_FilterInfo.AccTime / m_FilterInfo.Duration;
		Alpha = (UINT)(255.f * fRatio);
	}

	else if (FILTER_EFFECT::FADE_IN == m_FilterInfo.Effect)
	{
		float fRatio = 1.f - (m_FilterInfo.AccTime / m_FilterInfo.Duration);
		Alpha = (UINT)(255.f * fRatio);
	}

	m_FilterInfo.AccTime += DT;
	if (m_FilterInfo.Duration < m_FilterInfo.AccTime)
	{
		m_FilterInfo.Effect = NONE;
		m_AccTime = 0.f;
		m_Duration = 0.f;
	}

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = Alpha;
	blend.AlphaFormat = 0;

	AlphaBlend(BackDC, 0, 0
		, m_BlackFilterTex->GetWidth()
		, m_BlackFilterTex->GetHeight()
		, m_BlackFilterTex->GetDC()
		, 0, 0
		, m_BlackFilterTex->GetWidth()
		, m_BlackFilterTex->GetHeight()
		, blend);
}

void CCamera::CameraEffect()
{
	if (false == m_CamShake)
		return;

	// ī�޶� �̵� ����(1.f == �Ʒ�, -1.f == ��)	
	m_ShakeOffset.y += m_Amplitude * 4.f * m_Frequency * DT * m_Dir;

	// ���� ��ġ�� �Ѿ�� ������ ��ȯ�����ش�.
	if (m_Amplitude < fabs(m_ShakeOffset.y))
	{
		// �ʰ����� �ٽ� �������� �����Ų��.
		m_ShakeOffset.y += (fabs(m_ShakeOffset.y) - m_Amplitude) * -m_Dir;

		// ���� ��ȯ
		m_Dir *= -1.f;
	}

	// ȿ�� �����ð��� �� �Ǿ����� üũ
	m_AccTime += DT;
	if (m_Duration < m_AccTime)
	{
		// ȿ���� ����Ǹ� ���� �� ����
		m_AccTime = 0.f;
		m_CamShake = false;
		m_ShakeOffset = Vec2(0.f, 0.f);
	}
}
