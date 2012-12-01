#include "stdafx.h"
#include "Img.h"

Img::Img(std::wstring src) {
	img_ = (HBITMAP)LoadImage(NULL, src.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	mask_img_ = CreateBitmapMask(img_, RGB(255, 255, 255));
}

Img::Img(HBITMAP img, HBITMAP img_mask) {
	img_ = img;
	mask_img_ = img_mask;
}

HBITMAP Img::CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent) {
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

void Img::Draw(HDC hDc, int x, int y, int width, int height) {
/*	
	HDC hNewDC = CreateCompatibleDC(hDc);

	BITMAP bm;
	GetObject(m_img, sizeof(BITMAP), &bm);

	if (width == -1) {
		// width = bm.bmWidth;
		width = 50;
	}

	if (height == -1) {
		// height = bm.bmHeight;
		height = 50;
	}

	SelectObject(hNewDC, m_mask_img);
	StretchBlt(hDc, x, y, width, height, hNewDC, 0, 0, 50, 50, SRCAND);

	HBITMAP hOrigBMP = (HBITMAP)SelectObject(hNewDC, m_img);
	StretchBlt(hDc, x, y, width, height, hNewDC, 0, 0, 50, 50, SRCPAINT);

	SelectObject(hNewDC, hOrigBMP);
	DeleteDC(hNewDC);
*/

	HDC hNewDC = CreateCompatibleDC(hDc);

	BITMAP bm;
	GetObject(img_, sizeof(BITMAP), &bm);

	if (width == -1) {
		width = bm.bmWidth;
	}

	if (height == -1) {
		height = bm.bmHeight;
	}

	SelectObject(hNewDC, mask_img_);
	StretchBlt(hDc, x, y, width, height, hNewDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);

	HBITMAP hOrigBMP = (HBITMAP)SelectObject(hNewDC, img_);
	StretchBlt(hDc, x, y, width, height, hNewDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT);

	SelectObject(hNewDC, hOrigBMP);
	DeleteDC(hNewDC);
}

void Img::set_img(HBITMAP img) {
	img_ = img;
}

void Img::set_img_mask(HBITMAP hbm) {
	mask_img_ = hbm;
}

Img* Img::MakeCopy() {
	return new Img(img_, mask_img_);
}