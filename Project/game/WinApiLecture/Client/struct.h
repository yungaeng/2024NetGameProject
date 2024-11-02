#pragma once


struct Vec2
{
	float x;
	float y;

public:
	float Length()	{	return sqrt(x * x + y * y);	}
	bool IsZero()
	{
		if (x == 0.f && y == 0.f)
			return true;
		return false;
	}
	Vec2& Normalize()
	{
		float fLen = Length();

		// 0으로 나누는 경우는 반드시 예외처리를 하자
	assert(fLen != 0.f);


		x /= fLen;
		y /= fLen;
			return *this;
	}

public:
	Vec2 operator - ()
	{
		return Vec2(-x, -y);
	}
	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}

	Vec2 operator + (Vec2 _vOther)
	{
		return Vec2(x + _vOther.x, y + _vOther.y);
	}
	void operator += (Vec2 _vOther)
	{
		x += _vOther.x;
		y += _vOther.y;
	}
	Vec2 operator - (Vec2 _vOther)
	{
		return Vec2(x - _vOther.x, y - _vOther.y);
	}
	void operator -= (Vec2 _vOther)
	{
		x -= _vOther.x;
		y -= _vOther.y;
	}
	void operator -= (float _f)
	{
		x -= _f;
		y -= _f;
	}
	Vec2 operator * (Vec2 _vOther)
	{
		return Vec2(x * _vOther.x, y * _vOther.y);
	}
	Vec2 operator * (int _i)
	{
		return Vec2(x * (float)_i, y * (float)_i);
	}
	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}
	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}
	Vec2 operator / (Vec2 _vOther)
	{

		// ★★★0으로 나누는 경우 주의
		assert(!(0.f == _vOther.x || 0.f == _vOther.y));

		return Vec2(x / _vOther.x, y / _vOther.y);
	}
	Vec2 operator / (float _f)
	{
		assert(!(0.f == _f));
		return Vec2(x / _f, y / _f);
	}

public:
	Vec2()
		:x(0.f)
		,y(0.f)
	{}
	Vec2(float _x, float _y)
		:x(_x)
		,y(_y)
	{}
	Vec2(int _x, int _y)
		:x((float)_x)
		,y((float)_y)
	{}

	Vec2(POINT _pt)
		:x((float)_pt.x)
		,y((float)_pt.y)
	{}
};
