#pragma once

class CObject;

enum class CAM_EFFECT
{
	FADE_IN,
	FADE_OUT,
	NONE,
};


class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_vLookAt;		// 실제 카메라의 중심 == 화면의 중심, 카메라가 보는 위치
	Vec2		m_vPrevLookAt;	// 카메라가 보는 이전 프레임 위치
	Vec2		m_vCurLookAt;	// 이전 위치와 현재위치 보정위치
	CObject*	m_pTargerObj;	// 카메라 타겟 오브젝트
	Vec2		m_vDiff;		// 해상도 중심위치와, 카메라 Lookat과의 차이값
	float		m_fTime;		// 타겟을 따라가는데 걸리는 시간
	float		m_fSpeed;		// 타겟을 따라가는 속도
	float		m_fAccTime;		// 누적시간

//	CAM_EFFECT	m_eEffect;

public:
	void SetLookAt(Vec2 _vLook) 
	{
		m_vLookAt = _vLook; 
		float fMoveDist = (m_vLookAt - m_vPrevLookAt).Length();
		m_fSpeed = fMoveDist / m_fTime;
		m_fAccTime = 0.f;
	}
	void SetTarget(CObject* _pTarget) {m_pTargerObj = _pTarget; }

	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) { return _vObjPos - m_vDiff; }		// 실제 좌표 -> 렌더링 좌표 반환
	Vec2 GetRealPos(Vec2 _RenderPos) { return _RenderPos + m_vDiff; }	// 실제 좌표 반환



public:
	void update();

private:
		void CalDiff();
};


