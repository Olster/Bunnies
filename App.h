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

	std::wstring app_name() const { return app_name_; }
	int window_width() const { return window_width_; }
	int window_height() const { return window_height_; }

	void OnCreate(HWND hwnd);
	int OnDestroy();
	void OnPaint(HWND hwnd);
	void OnTimer(HWND hwnd);
	void OnKeyboard(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	std::wstring app_name_;
	int window_height_;
	int window_width_;

	// These guys can be initialized right here
	static const int bunny_img_width_ = 50;
	static const int bunny_img_height_ = 50;

	Img background_;

	std::list<Bunny*> bunnies_;

	DISALLOW_COPY_AND_ASSING(App);
};

