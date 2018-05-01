#include "utils.h"

Key Utils::ConvertToKey(const LPARAM val)
{
	return (((Key)val >> 16) & 0xFFFF);
}

ModifierKey Utils::ConvertToModifier(const LPARAM val)
{
	return ((ModifierKey)val & 0xFFFF);
}

bool Utils::IsTopMost(HWND hwnd)
{
	WINDOWINFO info;
	GetWindowInfo(hwnd, &info);
	return (info.dwExStyle & WS_EX_TOPMOST) ? true : false;
}

bool Utils::IsFullscreenSize(HWND hwnd, const int cx, const int cy)
{
	RECT r;
	GetWindowRect(hwnd, &r);
	return r.right - r.left == cx && r.bottom - r.top == cy;
}

bool Utils::IsFullscreenMaximized(HWND hwnd)
{
	if (IsTopMost(hwnd))
	{
		const int cx = GetSystemMetrics(SM_CXSCREEN);
		const int cy = GetSystemMetrics(SM_CYSCREEN);
		if (IsFullscreenSize(hwnd, cx, cy))
			return true;
	}
	return false;
}