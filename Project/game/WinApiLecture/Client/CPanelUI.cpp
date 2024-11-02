#include "CPanelUI.h"
#include "CKeyMgr.h"


CPanelUI::CPanelUI()
	: CUI(false)	// 부모 생성자의 기본생성자가 없기 때문에 따로 인자 버전 호출을 해주어야 한다
{
}

CPanelUI::~CPanelUI()
{
}


void CPanelUI::update()
{

}

void CPanelUI::render(HDC _dc)
{
	CUI::render(_dc);
}


void CPanelUI::MouseOn()
{
	if (ISLbtnDown())
	{
		// 드래그 하는 차이 값
		Vec2 vDiff = MOUSE_POS - m_vDragStart;
		Vec2 vCurPos = GetPos();
		vCurPos += vDiff;
		SetPos(vCurPos);

		m_vDragStart = MOUSE_POS;
	}
}

void CPanelUI::MouseLbtnDown()
{
	m_vDragStart = MOUSE_POS;
}

void CPanelUI::MouseLbtnUp()
{
}

