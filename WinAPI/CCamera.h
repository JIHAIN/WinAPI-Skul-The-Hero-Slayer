#pragma once

enum FILTER_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};

struct FILTER_INFO
{
	FILTER_EFFECT	Effect;
	float			Duration;
	float			AccTime;
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		 m_LookAt;
	Vec2		 m_LookAtDiff;
	Vec2		 m_ShakeOffset;
	float		 m_CamSpeed;

	// 카메라가 플레이어 중심으로 움직이게
	Vec2         m_TargetPos;
	Vec2         m_MapMin;
	Vec2         m_MapMax;
	float        m_LerpSpeedX;
	float        m_LerpSpeedY;
	float        m_DefaultPlayerY;
	float        m_CameraAdjustThreshold;
	float        m_CameraAdjustSpeed;

	// Filter Effect
	CTexture*	 m_BlackFilterTex;
	FILTER_INFO	 m_FilterInfo;


	// Camera Oscillation
	float		 m_Frequency;
	float		 m_Amplitude;
	float		 m_Duration;
	float		 m_AccTime;
	float	  	 m_Dir;
	bool	 	 m_CamShake;

public:
	void SetCamShake(float _Frequency, float _Amplitude, float _Duration)
	{
		m_Frequency = _Frequency;
		m_Amplitude = _Amplitude;
		m_Duration = _Duration;
		m_Dir = 1.f;
		m_AccTime = 0.f;
		m_CamShake = true;
	}

	void SetFilterEffect(FILTER_EFFECT _Effect, float _Duration)
	{
		m_FilterInfo.Effect = _Effect;
		m_FilterInfo.Duration = _Duration;
		m_FilterInfo.AccTime = 0.f;
	}

	void SetLookAt(Vec2 _Look) { m_LookAt = _Look; }
	Vec2 GetLookAt() { return m_LookAt + m_ShakeOffset; }
	Vec2 GetRenderPos(Vec2 _RealPos) { return _RealPos - m_LookAtDiff; }
	Vec2 GetRealPos(Vec2 _vRenderPos) { return _vRenderPos + m_LookAtDiff; }

	// 플레이어 따라 움직이게
	void SetTargetPos(Vec2 _Target) { m_TargetPos = _Target; }
	void SetLerpSpeedX(float _Speed) { m_LerpSpeedX = _Speed; }
	void SetLerpSpeedY(float _Speed) { m_LerpSpeedY = _Speed; }
	void SetMapBounds(Vec2 _Min, Vec2 _Max) { m_MapMin = _Min; m_MapMax = _Max; }
	void SetDefaultPlayerY(float y) { m_DefaultPlayerY = y; }
	void SetCameraAdjustThreshold(float threshold) { m_CameraAdjustThreshold = threshold; }
	void SetCameraAdjustSpeed(float speed) { m_CameraAdjustSpeed = speed; }


	// 플레이어를 카메라 범위 내로 제한
	Vec2 PlayerMoveLimit(const Vec2& position);

public:
	void Init();
	void Tick();
	void Render();

private:
	void CameraEffect();
};
