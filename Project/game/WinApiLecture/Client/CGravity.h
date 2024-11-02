#pragma once
#include "CObject.h"

class CGravity
{
private:
	CObject*	m_pOwner;
		
	bool		m_bGround;
public:
	void SetGround(bool _b);
public:
	void finalupdate();
public:
	CGravity();
	~CGravity();

	friend class CObject;
};

