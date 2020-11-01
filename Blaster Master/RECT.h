#pragma once

class FRECT
{
public:
	float top, left, right, bottom;

	FRECT(float left = 0, float top = 0, float right = 0, float bottom = 0)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}

	bool operator==(FRECT other)
	{
		return this->left == other.left &&
			this->right == other.right &&
			this->top == other.top &&
			this->bottom == other.bottom;
	}

	// immutable object
	FRECT operator/(float x)
	{
		 return FRECT(left/x, top/x, right/x, bottom/x);
	}

	FRECT operator/(int x)
	{
		return FRECT(left / x, top / x, right / x, bottom / x);
	}
};
