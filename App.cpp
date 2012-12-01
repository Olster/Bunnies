#include "stdafx.h"
#include "App.h"

#include <ctime>

App::App(std::wstring app_name, int window_width, int window_height):
background_(L"images/background.bmp")
{
	srand(time(0));

	app_name_ = app_name;
	window_width_ = window_width;
	window_height_ = window_height;

	// Making initialy 5 bunnies
	for (int i = 0; i <= 5; i++) {
		bunnies_.push_back(new Bunny());
	}
}


App::~App() {
	for (std::list<Bunny*>::iterator i = bunnies_.begin(); i != bunnies_.end(); ++i) {
		Bunny* temp = *i;  // Returns |Bunny*|
		
		// Next line is optional
		//bunnies_.remove(temp);  // Removes bunny from the list

		delete temp;
	}
}

void App::OnCreate(HWND hwnd) {
	// Setting 6 sec timer
	SetTimer(hwnd, 1, 6000, NULL);
}

int App::OnDestroy() {
	PostQuitMessage(0);
	return 0;
}

void App::OnPaint(HWND hwnd) {
	PAINTSTRUCT ps;
	RECT client_rect;

	HDC hdc = BeginPaint(hwnd, &ps);

	GetClientRect(hwnd, &client_rect);

	// Copies of bitmaps for drawing on them
	HDC hdc_mem = CreateCompatibleDC(hdc);
	HBITMAP h_mem_bitmap = CreateCompatibleBitmap(hdc, client_rect.right, client_rect.bottom);

	SelectObject(hdc_mem, GetStockObject(WHITE_BRUSH));
	SelectObject(hdc_mem, h_mem_bitmap);

	Rectangle(hdc_mem, client_rect.left - 1, client_rect.top - 1, client_rect.right + 1, client_rect.bottom + 1);

	background_.Draw(hdc_mem, 0, 0, client_rect.right, client_rect.bottom);

	// Drawing here

	for (std::list<Bunny*>::iterator i = bunnies_.begin(); i != bunnies_.end(); ++i) {
		int x = rand() % (client_rect.right - bunny_img_width_);
		int y = rand() % (client_rect.bottom - bunny_img_height_ - 15);	// |15| for correct name displaying

		// TODO Make pic width and height in class
		(*i)->Draw(hdc_mem, x, y, 50, 50);
	}

	// Stop drawing

	StretchBlt(hdc, 0, 0, client_rect.right, client_rect.bottom, hdc_mem, 0, 0, client_rect.right, client_rect.bottom, SRCCOPY);

	DeleteObject(hdc_mem);
	DeleteObject(h_mem_bitmap);

	EndPaint(hwnd, &ps);
}

void App::OnTimer(HWND hwnd) {
	//MessageBox(hwnd, L"Time event", L"Timer", MB_OK);

	if (bunnies_.empty()) {
		MessageBox(hwnd, L"No bunnies left, closing the app", L"Oh well", MB_OK);
		OnDestroy();
	}

	if (bunnies_.size() > 50) {
		//KillHalf();

		// TODO Implement |KillHalf()| and handle it correctly
		MessageBox(NULL, L"Can't handle so many bunnies!", L"Oh well", MB_OK);
		PostQuitMessage(0);
		return;
	}

	std::list<Bunny*>::iterator i = bunnies_.begin();
	while (i != bunnies_.end()) {
		(*i)->MakeOlder();

		if ((*i)->get_age_() > 10) {
			// Bunny is too old and has to die :(

			// But if bunny is a hazardous vampire, it stays living unless it's more than 50 y. o.
			if ((*i)->is_hazardous_vampire() && ((*i)->get_age_() < 50)) {
				continue;
			}

			Bunny* temp = *i;

			// TODO Ask how to make it work properly

			// TODO Work on this later
			i = bunnies_.erase(i);

			delete temp;
			temp = 0;

			// Don't let it get increment the iterator
			continue;
		}

		// Making new bunny
		if ((*i)->is_male() && ((*i)->get_age_() > 2) && !(*i)->is_hazardous_vampire()) {
			for (std::list<Bunny*>::iterator j = bunnies_.begin(); j != bunnies_.end(); ++j) {
				// Looking for females
				// Sounds creepy, doesn't it?

				if (!(*j)->is_male() && ((*j)->get_age_() > 2) && !(*j)->is_hazardous_vampire()) {
					Bunny* new_bunny = new Bunny();

					// I'm sure this will lag when I'll delete objects
					Img* mother_img = (*j)->MakeImgCopy();
					new_bunny->set_img(mother_img);

					bunnies_.push_back(new_bunny);
				}
			}
		}

		++i;
	}

	InvalidateRect(hwnd, NULL, FALSE);
}

void App::OnKeyboard(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_KEYUP:
		break;

		case WM_KEYDOWN:
		break;

		case WM_CHAR: {
			switch (wParam) {
				case 'k':
					//KillHalf();
					PostQuitMessage(0);
				break;

				case 'x':
					PostQuitMessage(0);
				break;
			}
		}
		break;
	}
}