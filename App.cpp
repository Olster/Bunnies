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
	for (int i = 0; i < 5; i++) {
		bunnies_.push_back(new Bunny());
	}
}


App::~App() {
	for (std::list<Bunny*>::iterator i = bunnies_.begin(); i != bunnies_.end(); ++i) {
		Bunny* temp = *i;  // Returns |Bunny*|

		// Uncomment the following for the info about each bunny in the end
		/*std::wstring bunny_info = temp->get_name();

		if (temp->is_male()) {
			bunny_info += L" (m) is ";
		} else {
			bunny_info += L" (f) is ";
		}

		bunny_info += helpers::IntToString(temp->get_age());

		bunny_info += L" years old and is";

		if (!temp->is_hazardous_vampire()) {
			bunny_info += L" not ";
		}

		bunny_info += L" a hazardous vampire";

		MessageBox(NULL, bunny_info.c_str(), L"Look!", MB_OK);*/
		
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

	// Drawing number of bunnies on the screen
	DrawInfoText(hdc_mem);

	for (std::list<Bunny*>::iterator i = bunnies_.begin(); i != bunnies_.end(); ++i) {
		int x = rand() % (client_rect.right - bunny_img_width_);
		int y = rand() % (client_rect.bottom - bunny_img_height_ + 15);	// |15| for correct name displaying

		(*i)->Draw(hdc_mem, x, y, bunny_img_width_, bunny_img_height_);
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

	if (bunnies_.size() > 100) {
		KillHalf();
	}

	if (bunnies_.size() > 500) {
		MessageBox(NULL, L"Can't handle so many bunnies, they escape!", L"Damn", MB_OK);
		PostQuitMessage(0);
		return;
	}

	// Bunnies that will turn hazardous after this aging phase
	std::vector<Bunny*> turn_hazardous;

	std::list<Bunny*>::iterator i = bunnies_.begin();
	while (i != bunnies_.end()) {
		(*i)->MakeOlder();

		if ((*i)->get_age() > 10) {
			// Bunny is too old and has to die :(

			// But if bunny is a hazardous vampire, it stays living unless it's more than 50 y. o.
			if ((*i)->is_hazardous_vampire() && ((*i)->get_age() < 50)) {
				continue;
			}

			Bunny* temp = *i;

			i = bunnies_.erase(i);

			delete temp;
			temp = 0;

			// Don't let it get increment the iterator
			continue;
		}

		// Making new bunny
		if ((*i)->is_male() && ((*i)->get_age() > 2) && !(*i)->is_hazardous_vampire()) {
			for (std::list<Bunny*>::iterator j = bunnies_.begin(); j != bunnies_.end(); ++j) {
				// Looking for females
				// Sounds creepy, doesn't it?

				if (!(*j)->is_male() && ((*j)->get_age() > 2) && !(*j)->is_hazardous_vampire()) {
					Bunny* new_bunny = new Bunny();

					// I'm sure this will lag when I'll delete objects
					Img* mother_img = (*j)->MakeImgCopy();
					new_bunny->set_img(mother_img);

					bunnies_.push_back(new_bunny);
				}
			}
		}

		// Letting one hazardous vampire to transform the other one
		// Have to make temp vector of adresses to change them after selection
		if ((*i)->is_hazardous_vampire()) {
			for (std::list<Bunny*>::iterator j = bunnies_.begin(); j != bunnies_.end(); ++j) {
				if (!(*j)->is_hazardous_vampire()) {
					turn_hazardous.push_back(*j);

					// Transforming one bunny only
					break;
				}
			}
		}

		++i;
	}

	// Making selected bunnies hazardous
	for (std::vector<Bunny*>::iterator j = turn_hazardous.begin(); j != turn_hazardous.end(); ++j) {
		(*j)->MakeHazardousVampire();
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
					KillHalf();
				break;

				case 'x':
					PostQuitMessage(0);
				break;
			}
		}
		break;
	}
}

void App::DrawInfoText(HDC hdc, int x, int y) {
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

	old_font = (HFONT)::SelectObject(hdc, arial_font);

	std::wstring display_text = L"There are ";

	// I'm sure I can make this with bunnies_.size()
	display_text += helpers::IntToString(Bunny::get_bunnies_overall());

	display_text += L" bunnies";

	TextOutW(hdc, x, y, display_text.c_str(), display_text.length());

	SelectObject(hdc, old_font);

	DeleteObject(arial_font);
}

void App::KillHalf() {
	// Vector of bunnies to be deleted
	std::vector<Bunny*> poor_bunnies;

	std::list<Bunny*>::iterator i = bunnies_.begin();

	while (poor_bunnies.size() < bunnies_.size()/2) {
		// 50% chance to be selected for butchery
		if (rand() % 2) {
			poor_bunnies.push_back(*i);
			i = bunnies_.erase(i);

			continue;
		}

		++i;

		// Jumping back to the beginning
		if (i == bunnies_.end()) {
			i = bunnies_.begin();
		}
	}

	for (std::vector<Bunny*>::iterator i = poor_bunnies.begin(); i != poor_bunnies.end(); ++i) {
		delete *i;
	}

	std::wstring number_of_killed = helpers::IntToString(poor_bunnies.size());

	MessageBox(NULL, number_of_killed.c_str(), L"Were killed", MB_OK);
}