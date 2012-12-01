#pragma once

#include "copy_blocker.h"
#include <Windows.h>

class IEvent {
public:
	NO_DEFAULTS(IEvent);

	virtual void OnCreate(HWND hwnd) = 0;
	virtual int OnDestroy() = 0;
	virtual void OnPaint(HWND hwnd) = 0;
	virtual void OnTimer(HWND hwnd) = 0;
	virtual void OnKeyboard(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;

private:
	DISALLOW_COPY_AND_ASSING(IEvent);
};

