#pragma once
#include <Windows.h>
#include <vector>
#include <map>
#include <set>
#include <functional>

using std::vector;
using std::map;
using std::set;
typedef int Key;
typedef int ModifierKey;
typedef int KeyID;
struct Hotkey
{
	int ID;
	Key key;
	Key modifierKey;
};

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	void Update();
	void AddHotkey(const KeyID id, const Key key, const ModifierKey modifier, const std::function<void(KeyID)>& function);
	void RemoveHotkey(const KeyID id);

private:
	map<KeyID, std::function<void (KeyID)>> hotkeyList;
	static Key ConvertToKey(const LPARAM val);
	static Key ConvertToModifier(const LPARAM val);
};



