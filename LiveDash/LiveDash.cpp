#include "Plugin.h"
#include "LiveDash.h"


void LoggerConnect(EventArgs& e)
{
	
}

void LoggerDisconnect(EventArgs& e)
{

}

void LoggerUpdate(SampleDataUpdateEventArgs& e)
{
	
}

void OnStartStream()
{
	int x = 0;
}

void OnStopStream()
{
	int x = 0;
}

void OnStartStreaming()
{
	int x = 0;
}

void OnStopStreaming()
{
	int x = 0;
}

ImageSource* STDCALL CreateLDImage(XElement* data)
{
	return new Img(nullptr);
}
