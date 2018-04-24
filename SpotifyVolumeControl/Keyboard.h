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
	void AddHotkey(const Hotkey& hotkey, const std::function<void(void)>& function);
	void RemoveHotkey(const Hotkey& hotkey);

private:
	map<int, std::function<void (void)>> hotkeyList;
	static Key ConvertToKey(const LPARAM val);
	static Key ConvertToModifier(const LPARAM val);
};



