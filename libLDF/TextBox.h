#pragma once

#include "common.h"
#include "DashboardElement.h"

namespace libLDF
{
	// channel data display formatting types

	// The Textbox is the staple element in dashboards. It is used to display labels and variable values as text.
	class CTextBox :
		public CDashboardElement
	{
	public:
		CTextBox();
		virtual ~CTextBox();

		virtual void Init() {}

		// Renders the element into a bitmap with the data set by elements channel name
		// and sample buffer index
		virtual Gdiplus::Bitmap* Render(int sampleIndex);

		void SetFormatType(string& s);
		LabelFormatType GetFormatType() { return _format; }

		void SetLabel(string& s) { _label = s; }
		string& GetLabel() { return _label; }

		void SetDrawZero(string& s);
		void SetDrawZero(bool b) { _drawZero = b; }
		bool GetDrawZero() { return _drawZero; }

		void SetPrecision(int p) { _precision = p; }
		int GetPrecision() { return _precision; }

		void SetSigned(string& s);
		void SetSigned(bool b) { _signed = b; }
		bool GetSigned() { return _signed; }

	private:
		// Describes how the variable's value is displayed.
		LabelFormatType _format;
		// The text string to be displayed. "Label"
		std::string _label;
		// Show 0 when variable value = 0
		bool _drawZero;

		bool _signed;

		int _precision;
	};

}