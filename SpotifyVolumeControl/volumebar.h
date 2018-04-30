#pragma once
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#define WIDTH 66 //64
#define HEIGHT 144 //140
#define VOLUME_WIDTH 20
#define VOLUME_HEIGHT 84

using namespace Gdiplus;
namespace VolumeBar
{
	void DrawVolume(Gdiplus::Graphics* graphics, const int level);
}