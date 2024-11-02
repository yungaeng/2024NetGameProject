#pragma once
class CUI;


class CUIMgr
{
	SINGLE(CUIMgr);

private:
	CUI* m_pFocusedUI;

public:
	void update();
	// UI강제 포커스 함수
	void SetFocusdeUI(CUI* _pUI);

private:
	CUI* GetFocusedUI();
	CUI* GetTargetedUI(CUI* _pParentUI);	// 부모 UI 내에서 실제로 타겟팅 된 UI를 찾아 반환한다.


};

