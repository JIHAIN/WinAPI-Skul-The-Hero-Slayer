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
	, m_DefaultPlayerY(0.6f)  // 화면 높이의 60% 위치로 초기화
	, m_CameraAdjustThreshold(0.1f)  // 10%의 데드존 설정
	, m_CameraAdjustSpeed(0.5f)  // 카메라 조정 속도
	, m_BlackFilterTex(nullptr)
	, m_Frequency(0.f)
	, m_Amplitude(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
	, m_Dir(0.f)
	, m_CamShake(false)
{
	// Black Filter Texture 생성
	Vec2 vResolution = CEngine::Get()->GetResolution();
	m_BlackFilterTex = CAssetMgr::Get()->CreateTexture(L"BlackFliterTex", (UINT)vResolution.x, (UINT)vResolution.y);

	m_FilterInfo.Effect = NONE;
}

CCamera::~CCamera()
{
}

Vec2 CCamera::PlayerMoveLimit(const Vec2& position)
{
	// 카메라 중심 위치
	Vec2 cameraCenter = m_LookAt;

	// 화면 크기의 절반
	Vec2 halfScreenSize = CEngine::Get()->GetResolution() / 2.f;

	// 플레이어 콜라이더의 크기 (예시 값, 실제 크기로 조정 필요)
	Vec2 playerColliderSize(50.f, 60.f);

	// 플레이어가 화면 내에 완전히 들어오도록 여유 공간 설정
	Vec2 padding = playerColliderSize /2;

	// 카메라 범위 계산 (플레이어 콜라이더 고려)
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
	//레벨에디터 받아오기
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CLevel_Editor* pEditorLevel = dynamic_cast<CLevel_Editor*>(pCurLevel);
	// 만약 에디터 레벨이 아닌곳에서 메뉴바가 호출 됐다면 설계 오류고  널 포인트가 들어있을테니 어썰트를 건다.

	if (pEditorLevel) // 레벨 에디터 일떄
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

	else // 아닐떄
	{
		// 화면 해상도 및 화면 중앙 위치 계산
		Vec2 resolution = CEngine::Get()->GetResolution();
		Vec2 halfScreenSize = resolution / 2.f;

		// x축: 타겟 위치로 부드럽게 이동
		float newLookAtX = Lerp(m_LookAt.x, m_TargetPos.x, m_LerpSpeedX * DT);

		// y축: 플레이어의 높이에 따라 부드럽게 조정
		float playerScreenY = m_TargetPos.y - (m_LookAt.y - halfScreenSize.y);
		float normalizedPlayerY = playerScreenY / resolution.y;

		// 목표 Y 위치 계산
		float targetY = m_LookAt.y; // 기본적으로 현재 y 위치 유지

		// 부드러운 카메라 이동을 위한 데드존 설정
		float upperDeadzone = m_DefaultPlayerY - m_CameraAdjustThreshold;
		float lowerDeadzone = m_DefaultPlayerY + m_CameraAdjustThreshold;

		if (normalizedPlayerY < upperDeadzone)
		{
			// 플레이어가 상단 데드존을 벗어났을 때 부드럽게 위로 이동
			float diff = upperDeadzone - normalizedPlayerY;
			targetY = m_LookAt.y - (diff * resolution.y * m_CameraAdjustSpeed);
		}
		else if (normalizedPlayerY > lowerDeadzone)
		{
			// 플레이어가 하단 데드존을 벗어났을 때 부드럽게 아래로 이동
			float diff = normalizedPlayerY - lowerDeadzone;
			targetY = m_LookAt.y + (diff * resolution.y * m_CameraAdjustSpeed);
		}

		// 부드러운 Y축 이동 적용
		float newLookAtY = Lerp(m_LookAt.y, targetY, m_LerpSpeedY * DT);

		// 카메라 위치를 맵 범위 내로 제한
		newLookAtX = Clamp(newLookAtX, m_MapMin.x + halfScreenSize.x, m_MapMax.x - halfScreenSize.x);
		newLookAtY = Clamp(newLookAtY, m_MapMin.y + halfScreenSize.y, m_MapMax.y - halfScreenSize.y);

		// 새로운 카메라 위치 설정
		m_LookAt = Vec2(newLookAtX, newLookAtY);
		// 카메라 효과 동작
		CameraEffect();
	}


	// 카메라가 보고있는 지점과, 해상도 중심위치의 차이값
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

	// 카메라 이동 방향(1.f == 아래, -1.f == 위)	
	m_ShakeOffset.y += m_Amplitude * 4.f * m_Frequency * DT * m_Dir;

	// 진폭 수치를 넘어서면 방향을 전환시켜준다.
	if (m_Amplitude < fabs(m_ShakeOffset.y))
	{
		// 초과량을 다시 안쪽으로 적용시킨다.
		m_ShakeOffset.y += (fabs(m_ShakeOffset.y) - m_Amplitude) * -m_Dir;

		// 방향 전환
		m_Dir *= -1.f;
	}

	// 효과 유지시간이 다 되었는지 체크
	m_AccTime += DT;
	if (m_Duration < m_AccTime)
	{
		// 효과가 만료되면 세팅 값 정리
		m_AccTime = 0.f;
		m_CamShake = false;
		m_ShakeOffset = Vec2(0.f, 0.f);
	}
}
