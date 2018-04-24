#include "Keyboard.h"



Keyboard::Keyboard()
{
}


Keyboard::~Keyboard()
{
	for (auto const& v : hotkeyList)
		RemoveHotkey(v.first);
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
				v.second(v.first);
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


void Keyboard::AddHotkey(const KeyID id, Key key, ModifierKey modifier, const std::function<void(KeyID)>&function)
{
	RegisterHotKey(NULL, id, modifier, key);
	hotkeyList.emplace(std::pair<KeyID, std::function<void(KeyID)>>(id, function));
}

void Keyboard::RemoveHotkey(const KeyID id)
{
	UnregisterHotKey(NULL, id);
	hotkeyList.erase(id);
}
