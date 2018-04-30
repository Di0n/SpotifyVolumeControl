#include "volumebar.h"
#include <iostream>
#include <string>

void DrawVolumeBar(Gdiplus::Graphics* graphics)
{
	Rect rect(WIDTH / 3, HEIGHT / 6, VOLUME_WIDTH, VOLUME_HEIGHT);
	Pen pen(Color::LimeGreen, 1.0f);

	graphics->DrawRectangle(&pen, rect);
	graphics->FillRectangle(pen.GetBrush(), rect);
}

void DrawVolumeLevel(Gdiplus::Graphics* graphics, const int level)
{
	if (level < 0 || level > 100) return;
	const float ratio = (float)VOLUME_HEIGHT / (float)100;
	Rect rect(WIDTH / 3, HEIGHT / 6, VOLUME_WIDTH, VOLUME_HEIGHT - (ratio * level));

	Pen pen(Color::Gray, 1.0f);
	graphics->DrawRectangle(&pen, rect);
	graphics->FillRectangle(pen.GetBrush(), rect);
}

void DrawVolumeText(Gdiplus::Graphics* graphics, std::wstring wstr)
{
	Rect rect(0, 0, 20, 20);

	Font font(&FontFamily(L"Arial"), 12);
	LinearGradientBrush brush(rect, Color::White, Color::White, LinearGradientMode::LinearGradientModeHorizontal);
	graphics->DrawString(wstr.c_str(), -1, &font, PointF(20, 120), &brush);
}
void VolumeBar::DrawVolume(Gdiplus::Graphics* graphics, const int level)
{
	DrawVolumeBar(graphics);
	DrawVolumeLevel(graphics, level);
	DrawVolumeText(graphics, std::to_wstring(level));
}