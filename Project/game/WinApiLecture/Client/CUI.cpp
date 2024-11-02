#include "CUI.h"
#include "CCore.h"

#include "CCamera.h"
#include "CKeyMgr.h"
#include "SelectGDI.h"
#include "CTexture.h"


CUI::CUI(bool _bCamAff)
	:m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pTex(nullptr)
{
}

CUI::CUI(const CUI& _origin)
	:CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
	, m_pTex(nullptr)
{
	for (size_t i = 0; i < _origin.m_vecChildUI.size(); ++i)
	{
		AddChild(_origin.m_vecChildUI[i]->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}





void CUI::update()
{
	// 본인이 할 일


	// 이후 자식의 업데이트
	// child ui update

	update_child();
}

void CUI::render(HDC _dc)
{
	// 만약 카메라의 움직임에 따라 움직이는 UI라면 UI를 오버라이딩 받는 다른 UI class를 만들어서
	// render를 업데이트 시키자.
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();
	if (m_bCamAffected)
	{
		vPos = CCamera::GetInst()->GetRenderPos(vPos);
	}
	if (m_pTex != nullptr)
	{
		
				Vec2 vScale = GetScale();
				Vec2 vResoultion = CCore::GetInst()->GetResolution();
		
				//vPos = CCamera::GetInst()->GetRenderPos(vPos);
		
				float width = (float)m_pTex->Width();
				float height = (float)m_pTex->Height();
		
				TransparentBlt(_dc,
					vPos.x// - (float)(vScale.x / 2)
					, vPos.y// - (float)(vScale.y / 2)
					, vScale.x,  vScale.y
					, m_pTex->GetDC()
					, 0, 0, width, height
					, RGB(255, 0, 255));
	}
	else
	{
		if (m_bLbtnDown)
		{
			SelectGDI select(_dc, PEN_TYPE::GREEN);	Rectangle(_dc
				, (int)vPos.x
				, (int)vPos.y
				, (int)(vPos.x + vScale.x)
				, (int)(vPos.y + vScale.y));
		}
		else
		{
			Rectangle(_dc
				, (int)vPos.x
				, (int)vPos.y
				, (int)(vPos.x + vScale.x)
				, (int)(vPos.y + vScale.y));
		}
	}

	// child ui render
	render_child(_dc);

}

void CUI::update_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->update();
	}
}

void CUI::finalupdate()
{
	CObject::finalupdate();

	// UI의 최종 좌표를 구한다.
	m_vFinalPos = GetPos();

	// 자신에게 부모가 있다면	FinalPos에 ParentPos 를 더한다.
	if (GetParent())
	{
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}
	// UI Mouse 체크
	MouseOnCheck();

	
	finalupdate_child();
}

void CUI::finalupdate_child()
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->finalupdate();
	}
}

void CUI::render_child(HDC _dc)
{
	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->render(_dc);
	}
}

void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected)
	{
		// 카메라의 영향을 받는 UI 인 경우 카메라로부터 실제 좌표를 변경받아옴
		vMousePos = CCamera::GetInst()->GetRealPos(vMousePos);
	}
	if ((m_vFinalPos.x <= vMousePos.x && vMousePos.x <= m_vFinalPos.x + vScale.x)	// x축 체크
		&& (m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y))	// y축 체크 
	{
		m_bMouseOn = true;
	}
	else 
	{
		m_bMouseOn = false;
	}
}


void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}

