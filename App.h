#pragma once

#include <string>
#include <list>
#include "copy_blocker.h"

#include "IEvent.h"
#include "Img.h"
#include "Bunny.h"

class App : public IEvent {
public:
	App(std::wstring app_name, int window_width = 640, int window_height = 480);
	~App();

	std::wstring app_name() const { return m_app_name; }
	int window_width() const { return m_window_width; }
	int window_height() const { return m_window_height; }

	void OnCreate(HWND hwnd);
	int OnDestroy();
	void OnPaint(HWND hwnd);
	void OnTimer(HWND hwnd);
	void OnKeyboard(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Draws number of bunnies on the screen
	void DrawInfoText(HDC hdc, int x = 0, int y = 0);

	void KillHalf(HWND hwnd);

private:
	void AssignPositions();
	Position* FindFreePos();

	void FindBunnyPosInArray(const Position* const mother_pos, int& i_pos_index, int& j_pos_index);

	void MakeBunny(Img* mother_img, int i, int j);

	Bunny* FindBunnyByPos(Position pos);

	std::wstring m_app_name;
	int m_window_height;
	int m_window_width;

	// These guys can be initialized right here
	static const int m_bunny_img_width = 25;
	static const int m_bunny_img_height = 25;

	// 400 bunnies max
	Position m_positions[20][20];

	static bool m_positions_assigned;

	Img m_background;

	std::list<Bunny*> m_bunnies;

	DISALLOW_COPY_AND_ASSING(App);
};

