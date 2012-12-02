#include "StdAfx.h"
#include "Bunny.h"

#include <cstdlib>

std::vector<std::wstring> Bunny::list_of_names_ = Bunny::MakeNamesVector();
std::vector<std::wstring> Bunny::list_of_images_ = Bunny::MakeImgPathesVector();
int Bunny::bunnies_overall_ = 0;

std::vector<std::wstring> Bunny::MakeNamesVector() {
	std::vector<std::wstring> names;
	names.push_back(L"Alex");
	names.push_back(L"Bailey");
	names.push_back(L"Brett");
	names.push_back(L"Cameron");
	names.push_back(L"Casey");
	names.push_back(L"Chris");
	names.push_back(L"Cody");
	names.push_back(L"Dale");
	names.push_back(L"Dee");
	names.push_back(L"Drew");
	names.push_back(L"Evan");
	names.push_back(L"Hunter");

	return names;
}

std::vector<std::wstring> Bunny::MakeImgPathesVector() {
	std::vector<std::wstring> pathes;
	pathes.push_back(L"images/bunny0.bmp");
	pathes.push_back(L"images/bunny1.bmp");
	pathes.push_back(L"images/bunny2.bmp");
	pathes.push_back(L"images/bunny3.bmp");

	return pathes;
}

Bunny::Bunny():
image_(0)
{
	// Not sure why it's glitching on me
	// have to check if |names| and |pathes| are not empty
	// TODO Ask about it

	if (list_of_names_.empty()) {
		list_of_names_ = MakeNamesVector();
	}

	if (list_of_images_.empty()) {
		list_of_images_ = MakeImgPathesVector();
	}


	name_ = RandomName();

	// Static cast is easier to find than "(bool)"
	// 50% chance of male
	is_male_ = static_cast<bool>(rand() % 2);

	is_hazardous_vampire_ = RandomHazardous();

	age_ = 0;

	image_ = new Img(RandomImgPath());

	bunnies_overall_++;

	//MessageBox(NULL, name_.c_str(), L"New bunny was born", MB_OK);
}


Bunny::~Bunny() {
	delete image_;
	image_ = 0;

	bunnies_overall_--;

	std::wstring info_text = L"Bunny " + name_ + L" has died...";

	//MessageBox(NULL, info_text.c_str(), L"Sad news", MB_OK);
}

void Bunny::Draw(HDC hDc, int x, int y, int width, int height) {
	image_->Draw(hDc, x, y, width, height);

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

	std::wstring display_text = name_;

	display_text += L"(";
	display_text += helpers::IntToString(age_);
	display_text += L")";

	// It's really messy, must be another way
	// TODO Ask about the way out of this situation

	if (is_male_) {
		if (age_ > 2) {
			display_text += L"/M";
		} else {
			display_text += L"/m";
		}
	} else {
		if (age_ > 2) {
			display_text += L"/F";
		} else {
			display_text += L"/f";
		}
	}

	if (is_hazardous_vampire_) {
		display_text += L"/H";
	}

	TextOutW(hDc, x, y - 15, display_text.c_str(), display_text.length());				// "m_x + 2" for correct displaying

	SelectObject(hDc, old_font);

	DeleteObject(arial_font);
}


std::wstring Bunny::RandomName() {
	// Getting random positin in a vector of names
	int rand_num = rand() % list_of_names_.size();

	return list_of_names_[rand_num];
}

std::wstring Bunny::RandomImgPath() {
	int rand_num = rand() % list_of_images_.size();

	return list_of_images_[rand_num];
}

bool Bunny::RandomHazardous() {
	double rand_num = static_cast<double>(rand())/RAND_MAX;

	// Basically making it 2% chance of being hazardous
	if (rand_num < 0.02) {
		return true;
	}

	return false;
}

void Bunny::set_img(Img* img) {
	delete image_;
	image_ = 0;

	image_ = img;
}

int Bunny::get_bunnies_overall_() {
	return bunnies_overall_;
}