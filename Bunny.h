#pragma once

#include <string>
#include <vector>
#include "copy_blocker.h"

// Forward declaration
class Img;

struct Position {
	Position(int x = 0, int y = 0, int width = -1, int height = -1) {
		m_x = x;
		m_y = y;

		m_width = width;
		m_height = height;

		m_is_occupied = false;
	}

	bool operator==(const Position& pos) {
		return ((m_x == pos.m_x) && (m_y == pos.m_x));
	}

	int m_x;
	int m_y;

	int m_width;
	int m_height;

	bool m_is_occupied;
};

class Bunny {
public:
	Bunny();
	~Bunny();

	void Draw(HDC hDc);

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

	Position* get_pos() const { return m_pos; }
	void set_pos(Position* pos);

	static int get_bunnies_overall();

private:
	Position* m_pos;

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

