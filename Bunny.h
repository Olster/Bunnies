#pragma once

#include <string>
#include <vector>
#include "copy_blocker.h"
#include "Img.h"

class Bunny {
public:
	Bunny();
	~Bunny();

	// Returns list of names
	static std::vector<std::wstring> MakeNamesVector();

	// Returns bunny images paths
	static std::vector<std::wstring> MakeImgPathsVector();

	void Draw(HDC hDc, int x, int y, int width = -1, int height = -1);

	void MakeOlder() { age_++; };

	int get_age() const { return age_; }
	std::wstring get_name() const { return name_; }

	Img* get_img() const { return image_; }
	void set_img(Img* img);

	bool is_male() const { return is_male_; }
	bool is_hazardous_vampire() const { return is_hazardous_vampire_; }

	void MakeHazardousVampire() { is_hazardous_vampire_ = true; }

	Img* MakeImgCopy() const { return image_->MakeCopy(); }

	static int get_bunnies_overall();

private:
	std::wstring name_;
	bool is_male_;
	short age_;
	bool is_hazardous_vampire_;

	Img* image_;

	static int bunnies_overall_;

	static std::vector<std::wstring> list_of_names_;
	static std::vector<std::wstring> list_of_images_;

	bool RandomHazardous();

	// Returns random name from the list of names
	std::wstring RandomName();

	// Retuns path to random bunny img
	std::wstring RandomImgPath();

	DISALLOW_COPY_AND_ASSING(Bunny);
};

