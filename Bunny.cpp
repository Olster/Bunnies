#include "StdAfx.h"

#include "Img.h"
#include "Bunny.h"

#include <cstdlib>

int Bunny::m_bunnies_overall = 0;

Bunny::Bunny():
m_image(0)
{
	m_name = RandomName();

	// 50% chance of male
	m_is_male = (rand() % 2 == 0);

	m_is_hazardous_vampire = RandomHazardous();

	m_age = 0;

	m_image = new Img(RandomImgPath());

	m_bunnies_overall++;

	//MessageBox(NULL, m_name.c_str(), L"New bunny was born", MB_OK);
}


Bunny::~Bunny() {
	delete m_image;
	m_image = 0;

	m_bunnies_overall--;

	//std::wstring info_text = L"Bunny " + m_name + L" has died...";

	//MessageBox(NULL, info_text.c_str(), L"Sad news", MB_OK);
}

void Bunny::Draw(HDC hDc, int x, int y, int width, int height) {
	m_image->Draw(hDc, x, y, width, height);

	// Let's write bunny's name, shall we?
	HFONT arial_font;
	HFONT old_font;
	LOGFONT lf;

	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfHeight = -10;
	lstrcpy(lf.lfFaceName, L"Arial");
	arial_font = CreateFontIndirect(&lf);

	old_font = (HFONT)::SelectObject(hDc, arial_font);

	std::wstring display_text = m_name;

	display_text += L"(";
	display_text += helpers::IntToString(m_age);
	display_text += L")";

	if (m_is_male) {
		if (m_age > 2) {
			display_text += L"/M";
		} else {
			display_text += L"/m";
		}
	} else {
		if (m_age > 2) {
			display_text += L"/F";
		} else {
			display_text += L"/f";
		}
	}

	if (m_is_hazardous_vampire) {
		display_text += L"/H";
	}

	TextOutW(hDc, x, y - 15, display_text.c_str(), display_text.length());

	SelectObject(hDc, old_font);

	DeleteObject(arial_font);
}


std::wstring Bunny::RandomName() {
	std::wstring names[12] = {
		L"Alex",
		L"Bailey",
		L"Brett",
		L"Cameron",
		L"Casey",
		L"Chris",
		L"Cody",
		L"Dale",
		L"Dee",
		L"Drew",
		L"Evan",
		L"Hunter"
	};

	return names[rand() % 12];
}

std::wstring Bunny::RandomImgPath() {
	std::wstring images_paths[4] = {
		L"images/bunny0.bmp",
		L"images/bunny1.bmp",
		L"images/bunny2.bmp",
		L"images/bunny3.bmp"
	};

	return images_paths[rand() % 4];
}

bool Bunny::RandomHazardous() {
	int percentage = rand() % 100;

	// Basically making it 2% chance of being hazardous
	if (percentage < 2) {
		return true;
	}

	return false;
}

void Bunny::set_img(Img* img) {
	delete m_image;
	m_image = 0;

	m_image = img;
}

Img* Bunny::MakeImgCopy() const {
	return m_image->MakeCopy();
}

int Bunny::get_bunnies_overall() {
	return m_bunnies_overall;
}