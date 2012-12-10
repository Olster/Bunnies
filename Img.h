#pragma once

#include <string>
#include "copy_blocker.h"

class Img {
public:
	Img(std::wstring src);
	Img(HBITMAP img, HBITMAP img_mask);
	~Img() {}

	void set_img(HBITMAP img);
	void set_img_mask(HBITMAP hbm);

	void Draw(HDC hDc, int x, int y, int width = -1, int height = -1);

	Img* MakeCopy();

	static HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent);

private:
	HBITMAP m_img;
	HBITMAP m_mask_img;

	DISALLOW_COPY_AND_ASSING(Img);
};

