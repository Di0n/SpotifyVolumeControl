#pragma once
#include <Windows.h>
#include <map>
#include <functional>

using std::map;
typedef int Key;
typedef int ModifierKey;
typedef int KeyID;

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	void Update();
	void AddHotkey(const KeyID id, const Key key, const ModifierKey modifier, const std::function<void(KeyID)>& function);
	void RemoveHotkey(const KeyID id);
	static Key ConvertToKey(const LPARAM val);
	static ModifierKey ConvertToModifier(const LPARAM val);
private:
	map<KeyID, std::function<void (KeyID)>> hotkeyList;
	
};



