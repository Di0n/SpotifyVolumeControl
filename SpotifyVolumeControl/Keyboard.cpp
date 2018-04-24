#include "Keyboard.h"



Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
}

void Keyboard::Update()
{
	MSG msg = { 0 };

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
	{
		if (msg.message != WM_HOTKEY) return;

		const WORD value = LOWORD(msg.wParam);

		for (auto const& v : hotkeyList)
		{
			if (value == v.first)
			{
				v.second();
			}
		}
	}
	/*if (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message != WM_HOTKEY) return;

		const WORD value = LOWORD(msg.wParam);

		for (auto const& v : hotkeyList)
		{
			if (value == v.first)
			{
				v.second();
			}
		}
	}*/
}


void Keyboard::AddHotkey(const Hotkey& hotkey, const std::function<void(void)>&function)
{
	RegisterHotKey(NULL, hotkey.ID, hotkey.modifierKey, hotkey.key);
	hotkeyList.emplace(std::pair<int, std::function<void(void)>>(hotkey.ID, function));
}

void Keyboard::RemoveHotkey(const Hotkey& hotkey)
{
	UnregisterHotKey(NULL, hotkey.ID);
}
