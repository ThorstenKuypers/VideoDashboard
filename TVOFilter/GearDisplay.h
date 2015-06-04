#pragma once

#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

class GearDisplay
{
private:

	PointF m_displaySize;
	Point m_pos;
	Pen* m_digitBkg;	// digit Background
	Pen* m_digit;		
	SolidBrush* m_bkg;	// display background
	float m_margin;
	Graphics* m_gfx;
	Graphics* m_paintGfx;

	Bitmap* m_display;

	void DrawR();
	void Draw0();
	void Draw1();
	void Draw2();
	void Draw3();
	void Draw4();
	void Draw5();
	void Draw6();
	void Draw7();
	void DrawDigitBkg();

public:
	GearDisplay(Point displayPos, SizeF displaySize, float margin, float lineWidth, Graphics* paintGfx);

	~GearDisplay();

	void DrawDisplay(int gear);

};
