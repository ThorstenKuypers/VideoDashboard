
#include "Channel.h"

using namespace libDataLogging::DataLogger;


CBaseChannel::CBaseChannel() {}
CBaseChannel::~CBaseChannel() {}

std::string& CBaseChannel::GetName()
{
	return _name;
}

std::string& CBaseChannel::GetDescription()
{
	return _description;
}

std::string& CBaseChannel::GetUnit()
{
	return _unit;
}

int& CBaseChannel::GetType()
{
	return type;
}

