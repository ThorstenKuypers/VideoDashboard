
#include "libLDF.h"
#include "Dashboard.h"

using namespace libLDF;

const std::map<std::string, Gdiplus::Color> X11Colors::colors =
{
	// https://en.wikipedia.org/wiki/Web_colors#X11_color_names

	x11Color{ "Pink", Gdiplus::Color(0xFF, 0xC0, 0xCB) },
	x11Color{ "LightPink", Gdiplus::Color(0xFF, 0xB6, 0xC1) },
	x11Color{ "HotPink", Gdiplus::Color(0xFF, 0x69, 0xB4) },
	x11Color{ "DeepPink", Gdiplus::Color(0xFF, 0x14, 0x93) },
	x11Color{ "PaleVioletRed", Gdiplus::Color(0xFF, 0x70, 0x93) },
	x11Color{ "MediumVioletRed", Gdiplus::Color(0xC7, 0x15, 0x85) },

	x11Color{ "LightSalmon", Gdiplus::Color(0xFF, 0xA0, 0x7A) },
	x11Color{ "Salmon", Gdiplus::Color(0xFA, 0x80, 0x72) },
	x11Color{ "DarkSalmon", Gdiplus::Color(0xE9, 0x96, 0x7A) },
	x11Color{ "LightCoral", Gdiplus::Color(0xF0, 0x80, 0x80) },
	x11Color{ "IndianRed", Gdiplus::Color(0xCD, 0x5C, 0x5C) },
	x11Color{ "Crimson", Gdiplus::Color(0xDC, 0x14, 0x3C) },
	x11Color{ "FireBrick", Gdiplus::Color(0xB2, 0x22, 0x22) },
	x11Color{ "DarkRed", Gdiplus::Color(0x8B, 0x00, 0x00) },
	x11Color{ "Red", Gdiplus::Color(0xFF, 0x00, 0x00) },

	x11Color{ "OrangeRed", Gdiplus::Color(0xFF, 0x45, 0x00) },
	x11Color{ "Tomato", Gdiplus::Color(0xFF, 0x63, 0x47) },
	x11Color{ "Coral", Gdiplus::Color(0xFF, 0x7F, 0x50) },
	x11Color{ "DarkOrange", Gdiplus::Color(0xFF, 0x8C, 0x00) },
	x11Color{ "Orange", Gdiplus::Color(0xFF, 0xA5, 0x00) },

	x11Color{ "Yellow", Gdiplus::Color(0xFF, 0xFF, 0x00) },
	x11Color{ "LightYellow", Gdiplus::Color(0xFF, 0xFF, 0xE0) },
	x11Color{ "LemonChiffon", Gdiplus::Color(0xFF, 0xFA, 0xCD) },
	x11Color{ "LightGoldenrodYellow", Gdiplus::Color(0xFA, 0xFA, 0xD2) },
	x11Color{ "PapayaWhip", Gdiplus::Color(0xFF, 0xEF, 0xD5) },
	x11Color{ "Moccasin", Gdiplus::Color(0xFF, 0xE4, 0xB5) },
	x11Color{ "PeachPuff", Gdiplus::Color(0xFF, 0xDA, 0xB9) },
	x11Color{ "PaleGoldenrod", Gdiplus::Color(0xEE, 0xE8, 0xAA) },
	x11Color{ "Khaki", Gdiplus::Color(0xF0, 0xE6, 0x8C) },
	x11Color{ "DarkKhaki", Gdiplus::Color(0xBD, 0xB7, 0x6B) },
	x11Color{ "Gold", Gdiplus::Color(0xFF, 0xD7, 0x00) },

	x11Color{ "Cornsilk", Gdiplus::Color(0xFF, 0xF8, 0xDC) },
	x11Color{ "BlanchedAlmond", Gdiplus::Color(0xFF, 0xEB, 0xCD) },
	x11Color{ "Bisque", Gdiplus::Color(0xFF, 0xE4, 0xC4) },
	x11Color{ "NavajoWhite", Gdiplus::Color(0xFF, 0xDE, 0xAD) },
	x11Color{ "Wheat", Gdiplus::Color(0xF5, 0xDE, 0xB3) },
	x11Color{ "BurlyWood", Gdiplus::Color(0xDE, 0xB8, 0x87) },
	x11Color{ "Tan", Gdiplus::Color(0xD2, 0xB4, 0x8C) },
	x11Color{ "RosyBrown", Gdiplus::Color(0xBC, 0x8F, 0x8F) },
	x11Color{ "SandyBrown", Gdiplus::Color(0xF4, 0xA4, 0x60) },
	x11Color{ "Goldenrod", Gdiplus::Color(0xDA, 0xA5, 0x20) },
	x11Color{ "DarkGoldenrod", Gdiplus::Color(0xB8, 0x86, 0x0B) },
	x11Color{ "Peru", Gdiplus::Color(0xCD, 0x85, 0x3F) },
	x11Color{ "Chocolate", Gdiplus::Color(0xD2, 0x69, 0x1E) },
	x11Color{ "SaddleBrown", Gdiplus::Color(0x8B, 0x45, 0x13) },
	x11Color{ "Sienna", Gdiplus::Color(0xA0, 0x52, 0x2D) },
	x11Color{ "Brown", Gdiplus::Color(0xA5, 0x2A, 0x2A) },
	x11Color{ "Maroon", Gdiplus::Color(0x80, 0x00, 0x00) },

	x11Color{ "DarkOliveGreen", Gdiplus::Color(85, 107, 47) },
	x11Color{ "Olive", Gdiplus::Color(128, 128, 0) },
	x11Color{ "OliveDrab", Gdiplus::Color(107, 142, 35) },
	x11Color{ "YellowGreen", Gdiplus::Color(154, 205, 50) },
	x11Color{ "LimeGreen", Gdiplus::Color(50, 205, 50) },
	x11Color{ "Lime", Gdiplus::Color(0, 255, 0) },
	x11Color{ "LawnGreen", Gdiplus::Color(124, 252, 0) },
	x11Color{ "Chartreuse", Gdiplus::Color(127, 255, 0) },
	x11Color{ "GreenYellow", Gdiplus::Color(173, 255, 47) },
	x11Color{ "SpringGreen", Gdiplus::Color(0, 255, 127) },
	x11Color{ "MediumSpringGreen", Gdiplus::Color(0, 250, 154) },
	x11Color{ "LightGreen", Gdiplus::Color(144, 238, 144) },
	x11Color{ "PaleGreen", Gdiplus::Color(152, 251, 152) },
	x11Color{ "DarkSeaGreen", Gdiplus::Color(143, 188, 143) },
	x11Color{ "MediumSeaGreen", Gdiplus::Color(60, 179, 113) },
	x11Color{ "SeaGreen", Gdiplus::Color(46, 139, 87) },
	x11Color{ "ForestGreen", Gdiplus::Color(34, 139, 34) },
	x11Color{ "Green", Gdiplus::Color(0, 128, 0) },
	x11Color{ "DarkGreen", Gdiplus::Color(0, 100, 0) },

	x11Color{ "MediumAquamarine", Gdiplus::Color(102, 205, 170) },
	x11Color{ "Aqua", Gdiplus::Color(0, 255, 255) },
	x11Color{ "Cyan", Gdiplus::Color(0, 255, 255) },
	x11Color{ "LightCyan", Gdiplus::Color(224, 255, 255) },
	x11Color{ "PaleTurquoise", Gdiplus::Color(175, 238, 238) },
	x11Color{ "Aquamarine", Gdiplus::Color(127, 255, 212) },
	x11Color{ "Turquoise", Gdiplus::Color(64, 224, 208) },
	x11Color{ "MediumTurquoise", Gdiplus::Color(72, 209, 204) },
	x11Color{ "DarkTurquoise", Gdiplus::Color(0, 206, 209) },
	x11Color{ "LightSeaGreen", Gdiplus::Color(32, 178, 170) },
	x11Color{ "CadetBlue", Gdiplus::Color(95, 158, 160) },
	x11Color{ "DarkCyan", Gdiplus::Color(0, 139, 139) },
	x11Color{ "Teal", Gdiplus::Color(0, 128, 128) },

	x11Color{ "LightSteelBlue", Gdiplus::Color(176, 196, 222) },
	x11Color{ "PowderBlue", Gdiplus::Color(176, 224, 230) },
	x11Color{ "LightBlue", Gdiplus::Color(173, 216, 230) },
	x11Color{ "SkyBlue", Gdiplus::Color(135, 206, 235) },
	x11Color{ "LightSkyBlue", Gdiplus::Color(135, 206, 250) },
	x11Color{ "DeepSkyBlue", Gdiplus::Color(0, 191, 255) },
	x11Color{ "DodgerBlue", Gdiplus::Color(30, 144, 255) },
	x11Color{ "CornflowerBlue", Gdiplus::Color(100, 149, 237) },
	x11Color{ "SteelBlue", Gdiplus::Color(70, 130, 180) },
	x11Color{ "RoyalBlue", Gdiplus::Color(65, 105, 225) },
	x11Color{ "Blue", Gdiplus::Color(0, 0, 255) },
	x11Color{ "MediumBlue", Gdiplus::Color(0, 0, 205) },
	x11Color{ "DarkBlue", Gdiplus::Color(0, 0, 139) },
	x11Color{ "Navy", Gdiplus::Color(0, 0, 128) },
	x11Color{ "MidnightBlue", Gdiplus::Color(25, 25, 112) },

	x11Color{ "Lavender", Gdiplus::Color(230, 230, 250) },
	x11Color{ "Thistle", Gdiplus::Color(216, 191, 216) },
	x11Color{ "Plum", Gdiplus::Color(221, 160, 221) },
	x11Color{ "Violet", Gdiplus::Color(238, 130, 238) },
	x11Color{ "Orchid", Gdiplus::Color(218, 112, 214) },
	x11Color{ "Fuchsia", Gdiplus::Color(255, 0, 255) },
	x11Color{ "Magenta", Gdiplus::Color(255, 0, 255) },
	x11Color{ "MediumOrchid", Gdiplus::Color(186, 85, 211) },
	x11Color{ "MediumPurple", Gdiplus::Color(147, 112, 219) },
	x11Color{ "BlueViolet", Gdiplus::Color(138, 43, 226) },
	x11Color{ "DarkViolet", Gdiplus::Color(148, 0, 211) },
	x11Color{ "DarkOrchid", Gdiplus::Color(153, 50, 204) },
	x11Color{ "DarkMagenta", Gdiplus::Color(139, 0, 139) },
	x11Color{ "Purple", Gdiplus::Color(128, 0, 128) },
	x11Color{ "Indigo", Gdiplus::Color(75, 0, 130) },
	x11Color{ "DarkSlateBlue", Gdiplus::Color(72, 61, 139) },
	x11Color{ "RebeccaPurple", Gdiplus::Color(102, 51, 153) },
	x11Color{ "SlateBlue", Gdiplus::Color(106, 90, 205) },
	x11Color{ "MediumSlateBlue", Gdiplus::Color(123, 104, 238) },

	x11Color{ "White", Gdiplus::Color(255, 255, 255) },
	x11Color{ "Snow", Gdiplus::Color(255, 250, 250) },
	x11Color{ "Honeydew", Gdiplus::Color(240, 255, 240) },
	x11Color{ "MintCream", Gdiplus::Color(245, 255, 250) },
	x11Color{ "Azure", Gdiplus::Color(240, 255, 255) },
	x11Color{ "AliceBlue", Gdiplus::Color(240, 248, 255) },
	x11Color{ "GhostWhite", Gdiplus::Color(248, 248, 255) },
	x11Color{ "WhiteSmoke", Gdiplus::Color(245, 245, 245) },
	x11Color{ "Seashell", Gdiplus::Color(255, 245, 238) },
	x11Color{ "Beige", Gdiplus::Color(245, 245, 220) },
	x11Color{ "OldLace", Gdiplus::Color(253, 245, 230) },
	x11Color{ "FloralWhite", Gdiplus::Color(255, 250, 240) },
	x11Color{ "Ivory", Gdiplus::Color(255, 255, 240) },
	x11Color{ "AntiqueWhite", Gdiplus::Color(250, 235, 215) },
	x11Color{ "Linen", Gdiplus::Color(250, 240, 230) },
	x11Color{ "LavenderBlush", Gdiplus::Color(255, 240, 245) },
	x11Color{ "MistyRose", Gdiplus::Color(255, 228, 225) },

	x11Color{ "Gainsboro", Gdiplus::Color(220, 220, 220) },
	x11Color{ "LightGrey", Gdiplus::Color(211, 211, 211) },
	x11Color{ "Silver", Gdiplus::Color(192, 192, 192) },
	x11Color{ "DarkGray", Gdiplus::Color(169, 169, 169) },
	x11Color{ "Gray", Gdiplus::Color(128, 128, 128) },
	x11Color{ "DimGray", Gdiplus::Color(105, 105, 105) },
	x11Color{ "LightSlateGray", Gdiplus::Color(119, 136, 153) },
	x11Color{ "SlateGray", Gdiplus::Color(112, 128, 144) },
	x11Color{ "DarkSlateGray", Gdiplus::Color(47, 79, 79) },
	x11Color{ "Black", Gdiplus::Color(0, 0, 0) },
};

IDashboardLayout* LayoutInstanceFactory::GetDashboardLayoutInst()
{
	IDashboardLayout* ptr = nullptr;

	try {
		ptr = static_cast<IDashboardLayout*>(new CDashboard());
	}
	catch (std::exception e)
	{
		throw e;
	}

	return ptr;
}

void LayoutInstanceFactory::ReleaseDashboardLayoutInstance(IDashboardLayout* inst)
{
	CDashboard* ptr = static_cast<CDashboard*>(inst);

	if (ptr != nullptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}