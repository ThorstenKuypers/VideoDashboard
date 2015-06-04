// libds.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "libds.h"

using namespace DShow;
using namespace DShow::VideoEncoding;

LIBDS_API struct DShow::IVideoEncoder* init_instance(int encoder)
{
	DShow::IVideoEncoder* p =NULL;

	if (encoder ==1) {
		p =(DShow::IVideoEncoder*)new CWMEncoder();
		if (p)
			return p;
	}

	return NULL;
}

LIBDS_API struct DShow::IVideoEncoder* init_WindowsMediaEncoder9(void)
{
	DShow::IVideoEncoder* p =(DShow::IVideoEncoder*)new CWMEncoder();

	return p;
}

LIBDS_API struct DShow::IVideoEncoder* init_RawVideoEncoder(void)
{
	DShow::IVideoEncoder* p =(DShow::IVideoEncoder*)new CRawAviEncoder();

	return p;
}

LIBDS_API void release_instance(struct DShow::IVideoEncoder* inst, int type)
{
	if (inst) {
		if (type ==1) {
			CWMEncoder* p =(CWMEncoder*)inst;

			delete p;
			p =NULL;
		}
		if (type ==2) {
			CRawAviEncoder* p =(CRawAviEncoder*)inst;

			delete p;
			p =NULL;
		}

		inst =NULL;
	}
}