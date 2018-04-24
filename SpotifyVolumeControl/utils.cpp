#include "utils.h"

Key Utils::ConvertToKey(const LPARAM val)
{
	return (((Key)val >> 16) & 0xFFFF);
}

Key Utils::ConvertToModifier(const LPARAM val)
{
	return ((Key)val & 0xFFFF);
}