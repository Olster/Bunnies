#include "stdafx.h"
#include "App.h"

#include <ctime>

bool App::m_positions_assigned = false;

App::App(std::wstring app_name, int window_width, int window_height):
m_background(L"images/background.bmp")
{
	if (!m_positions_assigned) {
		AssignPositions();

		m_positions_assigned = true;
	}

	srand(time(NULL));

	m_app_name = app_name;
	m_window_width = window_width;
	m_window_height = window_height;

	// Making 5 bunnies initially
	for (int i = 0; i < 5; i++) {
		Bunny* temp_bunny = new Bunny;
		
		temp_bunny->set_pos(FindFreePos());

		m_bunnies.push_back(temp_bunny);
	}
}


App::~App() {
	for (std::list<Bunny*>::iterator i = m_bunnies.begin(); i != m_bunnies.end(); ++i) {
		Bunny* temp = *i;

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
		//m_bunnies.remove(temp);  // Removes bunny from the list

		delete temp;
	}
}

void App::OnCreate(HWND hwnd) {
	// Setting 2 sec timer
	SetTimer(hwnd, 1, 2000, NULL);
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

	m_background.Draw(hdc_mem, 0, 0, client_rect.right, client_rect.bottom);

	// Drawing here

	// Drawing number of bunnies on the screen
	DrawInfoText(hdc_mem);

	for (std::list<Bunny*>::iterator i = m_bunnies.begin(); i != m_bunnies.end(); ++i) {
		(*i)->Draw(hdc_mem);
	}

	// Stop drawing

	StretchBlt(hdc, 0, 0, client_rect.right, client_rect.bottom, hdc_mem, 0, 0, client_rect.right, client_rect.bottom, SRCCOPY);

	DeleteObject(hdc_mem);
	DeleteObject(h_mem_bitmap);

	EndPaint(hwnd, &ps);
}

void App::OnTimer(HWND hwnd) {
	//MessageBox(hwnd, L"Time event", L"Timer", MB_OK);

	if (m_bunnies.empty()) {
		MessageBox(hwnd, L"No bunnies left, closing the app", L"Oh well", MB_OK);
		OnDestroy();
	}

	if (m_bunnies.size() > 200) {
		KillHalf(hwnd);
		return;
	}

	if (m_bunnies.size() > 400) {
		MessageBox(NULL, L"Can't handle so many bunnies, they escape!", L"Damn", MB_OK);
		OnDestroy();
		return;
	}

	// Bunnies that will turn hazardous after this aging phase
	std::vector<Bunny*> turn_hazardous;

	std::list<Bunny*>::iterator i = m_bunnies.begin();
	while (i != m_bunnies.end()) {
		(*i)->MakeOlder();

		if ((*i)->get_age() > 10) {
			// Bunny is too old and has to die :(

			// But if bunny is a hazardous vampire, it stays living unless it's more than 50 y. o.
			// Not really good looking if statement, rewrite later
			if (!(*i)->is_hazardous_vampire() || ((*i)->is_hazardous_vampire() && ((*i)->get_age() > 50))) {
				Bunny* temp = *i;

				int i_pos = 0;
				int j_pos = 0;

				FindBunnyPosInArray((*i)->get_pos(), i_pos, j_pos);

				m_positions[i_pos][j_pos].m_is_occupied = false;

				i = m_bunnies.erase(i);

				delete temp;
				temp = 0;

				// Don't let it to get to increment the iterator
				continue;
			}			
		}

		// Making new bunny
		if ((*i)->is_male() && ((*i)->get_age() > 2) && !(*i)->is_hazardous_vampire()) {
			for (std::list<Bunny*>::iterator j = m_bunnies.begin(); j != m_bunnies.end(); ++j) {
				// Looking for females
				// Sounds creepy, doesn't it?

				if (!(*j)->is_male() && ((*j)->get_age() > 2) && !(*j)->is_hazardous_vampire()) {

					Position* mother_pos = (*j)->get_pos();
					int i_mother_pos_index = 0;
					int j_mother_pos_index = 0;

					// ins: mother_pos; outs: i_mother_pos_index, j_mother_pos_index
					FindBunnyPosInArray(mother_pos, i_mother_pos_index, j_mother_pos_index);

					// Let's find a place for a baby, shall we?

					// I could easily use GOTO, but butt
					bool bunny_is_made = false;

					// Let's check if top is free
					if (i_mother_pos_index > 0 && !bunny_is_made && !m_positions[i_mother_pos_index - 1][j_mother_pos_index].m_is_occupied) {
						Img* mother_img = (*j)->MakeImgCopy();
						
						MakeBunny(mother_img, i_mother_pos_index - 1, j_mother_pos_index);

						bunny_is_made = true;
					}

					// Time to check the bottom
					if (i_mother_pos_index < 19 && !bunny_is_made && !m_positions[i_mother_pos_index + 1][j_mother_pos_index].m_is_occupied) {
						Img* mother_img = (*j)->MakeImgCopy();
						
						MakeBunny(mother_img, i_mother_pos_index + 1, j_mother_pos_index);

						bunny_is_made = true;
					}

					// Time to check left
					if (j_mother_pos_index > 0 && !bunny_is_made && !m_positions[i_mother_pos_index][j_mother_pos_index - 1].m_is_occupied) {
						Img* mother_img = (*j)->MakeImgCopy();
						
						MakeBunny(mother_img, i_mother_pos_index, j_mother_pos_index - 1);

						bunny_is_made = true;
					}

					// And the right
					if (j_mother_pos_index < 19 && !bunny_is_made && !m_positions[i_mother_pos_index][j_mother_pos_index + 1].m_is_occupied) {
						Img* mother_img = (*j)->MakeImgCopy();
						
						MakeBunny(mother_img, i_mother_pos_index, j_mother_pos_index + 1);

						bunny_is_made = true;
					}
				}
			}
		}

		// Letting one hazardous vampire to transform the other one
		// Have to make temp vector of addresses to change them after selection
		if ((*i)->is_hazardous_vampire()) {

			int i_pos_hazardous;
			int j_pos_hazardous;

			Position* hazardous_pos = (*i)->get_pos();

			FindBunnyPosInArray(hazardous_pos, i_pos_hazardous, j_pos_hazardous);

			// Let's check if top is not hazardous yet

			bool is_victim_found = false;

			if (i_pos_hazardous > 0 && !is_victim_found) {
				Bunny* temp_bunny = FindBunnyByPos(m_positions[i_pos_hazardous - 1][j_pos_hazardous]);

				if (temp_bunny) {
					turn_hazardous.push_back(temp_bunny);
					is_victim_found = true;
				}
			}

			// Time to check the bottom
			if (i_pos_hazardous < 19 && !is_victim_found) {
				Bunny* temp_bunny = FindBunnyByPos(m_positions[i_pos_hazardous + 1][j_pos_hazardous]);
				
				if (temp_bunny) {
					turn_hazardous.push_back(temp_bunny);
					is_victim_found = true;
				}
			}

			// Time to check left
			if (j_pos_hazardous > 0 && !is_victim_found) {
				Bunny* temp_bunny = FindBunnyByPos(m_positions[i_pos_hazardous][j_pos_hazardous - 1]);

				if (temp_bunny) {
					turn_hazardous.push_back(temp_bunny);
					is_victim_found = true;
				}
			}

			// And the right
			if (j_pos_hazardous < 19 && !is_victim_found) {
				Bunny* temp_bunny = FindBunnyByPos(m_positions[i_pos_hazardous][j_pos_hazardous + 1]);

				if (temp_bunny) {
					turn_hazardous.push_back(temp_bunny);
					is_victim_found = true;
				}
			}
		}

		++i;
	}

	// Making selected bunnies hazardous if the vector is not empty
	if (!turn_hazardous.empty()) {
		for (std::vector<Bunny*>::iterator j = turn_hazardous.begin(); j != turn_hazardous.end(); ++j) {
			(*j)->MakeHazardousVampire();
		}
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
					KillHalf(hwnd);
				break;

				case 'x':
					PostQuitMessage(0);
				break;

				// Temporary thing to fasten the program
				case ' ':
					OnTimer(hwnd);
				break;
			}
		}
		break;

		default:

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
	lf.lfHeight = -8;
	lstrcpy(lf.lfFaceName, L"Arial");
	arial_font = CreateFontIndirect(&lf);

	old_font = (HFONT)::SelectObject(hdc, arial_font);

	std::wstring display_text = L"There are ";

	// I'm sure I can make this with m_bunnies.size()
	display_text += helpers::IntToString(Bunny::get_bunnies_overall());

	display_text += L" bunnies";

	TextOutW(hdc, x, y, display_text.c_str(), display_text.length());

	SelectObject(hdc, old_font);

	DeleteObject(arial_font);
}

void App::KillHalf(HWND hwnd) {
	if (m_bunnies.empty()) {
		// No bunnies to be killed
		return;
	}

	// Vector of bunnies to be deleted
	std::vector<Bunny*> poor_bunnies;

	std::list<Bunny*>::iterator i = m_bunnies.begin();

	while (poor_bunnies.size() < m_bunnies.size()/2) {
		// 50% chance to be selected for butchery
		if (rand() % 2) {
			poor_bunnies.push_back(*i);
			i = m_bunnies.erase(i);

			continue;
		}

		++i;

		// Jumping back to the beginning
		if (i == m_bunnies.end()) {
			i = m_bunnies.begin();
		}
	}

	for (std::vector<Bunny*>::iterator i = poor_bunnies.begin(); i != poor_bunnies.end(); ++i) {
		int i_pos = 0;
		int j_pos = 0;

		FindBunnyPosInArray((*i)->get_pos(), i_pos, j_pos);

		m_positions[i_pos][j_pos].m_is_occupied = false;

		delete *i;
	}

	std::wstring number_of_killed = helpers::IntToString(poor_bunnies.size());

	InvalidateRect(hwnd, NULL, FALSE);

	MessageBox(NULL, number_of_killed.c_str(), L"Were killed", MB_OK);
}

void App::AssignPositions() {
	int current_x = 0;

	// X distance between 2 bunnies
	int x_dist = 10;

	// Text is drawn higher than the bunny
	int current_y = 20;

	for (int i = 0; i < 20; i++) {
		current_x = 0;
		for (int j = 0; j < 20; j++) {
			m_positions[i][j].m_x = current_x;
			m_positions[i][j].m_y = current_y;
			m_positions[i][j].m_width = m_bunny_img_width;
			m_positions[i][j].m_height = m_bunny_img_height;

			current_x += m_bunny_img_width;
			current_x += x_dist;
		}

		current_y += m_bunny_img_height;
	}
}

Position* App::FindFreePos() {
	// If any free found, return the 0 0 position
	Position* out = &m_positions[0][0];

	bool found = false;

	while (!found) {
		int i = rand() % 20;
		int j = rand() % 20;

		if (!m_positions[i][j].m_is_occupied) {
			out = &m_positions[i][j];
			found = true;
		}
	}

	return out;
}

void App::FindBunnyPosInArray(const Position* const mother_pos, int& i_pos_index, int& j_pos_index) {
	bool is_found = false;
	for (int i = 0; i < 20; i++) {
		if (is_found) {
			break;
		}

		for (int j = 0; j < 20; j++) {

			// operator== glitched
			if ((mother_pos->m_x == m_positions[i][j].m_x) && (mother_pos->m_y == m_positions[i][j].m_y)) {
				i_pos_index = i;
				j_pos_index = j;

				is_found = true;

				break;
			}
		}
	}
}

void App::MakeBunny(Img* mother_img, int i, int j) {
	Bunny* new_bunny = new Bunny();
						
	new_bunny->set_img(mother_img);
	new_bunny->set_pos(&m_positions[i][j]);

	m_bunnies.push_back(new_bunny);
}

Bunny* App::FindBunnyByPos(Position pos) {
	for (std::list<Bunny*>::iterator i = m_bunnies.begin(); i != m_bunnies.end(); ++i) {
		if (((*i)->get_pos()->m_x == pos.m_x) && ((*i)->get_pos()->m_y == pos.m_y)) {
			return *i;
		}
	}
}