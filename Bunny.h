#pragma once

#include <string>
#include <vector>
#include "copy_blocker.h"

// Forward declaration
class Img;

class Bunny {
public:
	Bunny();
	~Bunny();

	void Draw(HDC hDc, int x, int y, int width = -1, int height = -1);

	void MakeOlder() { ++m_age; };

	int get_age() const { return m_age; }
	std::wstring get_name() const { return m_name; }

	Img* get_img() const { return m_image; }
	void set_img(Img* img);

	bool is_male() const { return m_is_male; }
	bool is_hazardous_vampire() const { return m_is_hazardous_vampire; }

	void MakeHazardousVampire() { m_is_hazardous_vampire = true; }

	// Returns copy of bunny's image
	Img* MakeImgCopy() const;

	static int get_bunnies_overall();

private:
	bool RandomHazardous();

	// Returns random name from the list of names
	std::wstring RandomName();

	// Retuns path to random bunny img
	std::wstring RandomImgPath();

	std::wstring m_name;
	bool m_is_male;
	short m_age;
	bool m_is_hazardous_vampire;

	Img* m_image;

	static int m_bunnies_overall;

	DISALLOW_COPY_AND_ASSING(Bunny);
};

