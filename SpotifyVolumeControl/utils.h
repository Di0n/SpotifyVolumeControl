#pragma once
#include <Windows.h>
typedef int Key;
typedef int ModifierKey;
namespace Utils
{
	Key ConvertToKey(const LPARAM val);
	ModifierKey ConvertToModifier(const LPARAM val);
	bool IsTopMost(HWND hwnd);
	bool IsFullscreenSize(HWND hwnd, const int cx, const int cy);
	bool IsFullscreenMaximized(HWND hwnd);
}