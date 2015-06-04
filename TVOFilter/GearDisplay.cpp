
#include "GearDisplay.h"

CGearDisplay::GearDisplay(Point displayPos, SizeF displaySize, float margin, float lineWidth, Graphics* paintGfx)
{
		m_displaySize.X =displaySize.Width;
		m_displaySize.Y =displaySize.Height;

		m_pos =displayPos;
		m_margin =margin;
		m_paintGfx =paintGfx;

		m_bkg =new SolidBrush(Color::FromArgb(20,20,20));
		m_digitBkg =new Pen(Color::FromArgb(70, 255, 0, 0), lineWidth);
		m_digit =new Pen(Color::Red, lineWidth);

		m_digitBkg->LineJoin =Drawing2D::LineJoin::Bevel;
		m_digitBkg->StartCap =Drawing2D::LineCap::Triangle;
		m_digitBkg->EndCap =Drawing2D::LineCap::Triangle;

		m_digit->LineJoin =Drawing2D::LineJoin::Bevel;
		m_digit->StartCap =Drawing2D::LineCap::Triangle;
		m_digit->EndCap =Drawing2D::LineCap::Triangle;
}

CGearDisplay::~CGearDisplay(void)
{
		delete m_bkg;
		delete m_digitBkg;
		delete m_digit;
}

void DrawDisplay(int gear)
{
		m_display =gcnew Bitmap(m_displaySize.X, m_displaySize.Y);
		m_gfx =Graphics::FromImage(m_display);

		m_gfx->FillRectangle(m_bkg, RectangleF(0.0f, 0.0f, m_displaySize.X, m_displaySize.Y));

		DrawDigitBkg();

		switch (gear) {

			case -1:
				DrawR();
				break;

			case 0:
				Draw0();
				break;

			case 1:
				Draw1();
				break;

			case 2:
				Draw2();
				break;

			case 3:
				Draw3();
				break;

			case 4:
				Draw4();
				break;

			case 5:
				Draw5();
				break;

			case 6:
				Draw6();
				break;

			case 7:
				Draw7();
				break;

			default:
				break;

		m_paintGfx->DrawImage(m_display, m_pos);

		delete m_gfx;
		delete m_display;
	}
}

void DrawR()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 6
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 7
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void Draw0()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 4
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 6
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 7
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void Draw1()
	{
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void Draw2()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 4
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 7
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void Draw3()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 4
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
}

void Draw4()
{
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 6
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
}

void Draw5()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 4
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 6
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
}

void Draw6()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 4
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 5
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 6
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 7
		m_gfx->DrawLine(m_digit, PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void Draw7()
{
		// 1
		m_gfx->DrawLine(m_digit, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digit, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

void DrawDigitBkg()
{
		// 1
		m_gfx->DrawLine(m_digitBkg, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_margin));
		// 2
		m_gfx->DrawLine(m_digitBkg, PointF(m_displaySize.X - m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 3
		m_gfx->DrawLine(m_digitBkg, PointF(m_displaySize.X - m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_displaySize.X - m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
		// 4
		m_gfx->DrawLine(m_digitBkg, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y - m_margin),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y - m_margin));
		// 5
		m_gfx->DrawLine(m_digitBkg, PointF(m_margin + (m_digitBkg->Width/2) + 1, m_displaySize.Y/2),
										 PointF(m_displaySize.X - m_margin - (m_digitBkg->Width/2) - 1, m_displaySize.Y/2));
		// 6
		m_gfx->DrawLine(m_digitBkg, PointF(m_margin, m_margin + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) - (m_digitBkg->Width/2) - 1));
		// 7
		m_gfx->DrawLine(m_digitBkg, PointF(m_margin, m_displaySize.Y - (m_displaySize.Y/2) + (m_digitBkg->Width/2) + 1),
										 PointF(m_margin, m_displaySize.Y - m_margin - (m_digitBkg->Width/2) -1));
}

GearDisplay(Point displayPos, SizeF displaySize, float margin, float lineWidth, Graphics^ paintGfx)
{
		m_displaySize.X =displaySize.Width;
		m_displaySize.Y =displaySize.Height;

		m_pos =displayPos;
		m_margin =margin;
		m_paintGfx =paintGfx;

		m_bkg =gcnew SolidBrush(Color::FromArgb(20,20,20));
		m_digitBkg =gcnew Pen(Color::FromArgb(70, 255, 0, 0), lineWidth);
		m_digit =gcnew Pen(Color::Red, lineWidth);

		m_digitBkg->LineJoin =Drawing2D::LineJoin::Bevel;
		m_digitBkg->StartCap =Drawing2D::LineCap::Triangle;
		m_digitBkg->EndCap =Drawing2D::LineCap::Triangle;

		m_digit->LineJoin =Drawing2D::LineJoin::Bevel;
		m_digit->StartCap =Drawing2D::LineCap::Triangle;
		m_digit->EndCap =Drawing2D::LineCap::Triangle;

}

