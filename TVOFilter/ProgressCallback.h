#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVOFilter -- DirectShow Transform Filter implementation
//
//	Copyright 2011-2012 Thorsten Kuypers
//	All Rights Reserved
//
///////////////////////////////////////////////////////////////////////////////

namespace TVOFilter {

struct IProgressCallback
{
	virtual void ProgressNotify(int curProgress) =0;
};

}