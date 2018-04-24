#pragma once
#include <Windows.h>
typedef int Key;
namespace Utils
{
	Key ConvertToKey(const LPARAM val);
	Key ConvertToModifier(const LPARAM val);
}