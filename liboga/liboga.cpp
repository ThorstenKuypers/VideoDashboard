
#include "liboga.h"
#include "IracingBinaryTelemetryFile.h"
#include "LiveDataLogger.h"

// implementation of common library wide interfaces and classes as well
// as global functions

using namespace libOGA;
using namespace libOGA::FileLogs;
using namespace libOGA::LiveLogs;

#pragma region //Sample Value

#pragma endregion


#pragma region // CDataChannel

CDataChannel::CDataChannel() :
_name(),
_unit(),
_desc(),
_key(0),
_type(0),
_offset(0)
{}

CDataChannel::CDataChannel(std::string name,
	std::string unit,
	std::string desc,
	int key,
	int type,
	int offset,
	ChannelType ct)
{
	_name = name;
	_unit = unit;
	_desc = desc;
	_key = key;
	_type = type;
	_offset = offset;
	_channelType = ct;
}
CDataChannel::~CDataChannel() {}

CDataChannel::CDataChannel(CDataChannel&& t) :
_name(t._name),
_desc(t._desc),
_unit(t._unit),
_type(t._type),
_key(t._key),
_offset(t._offset),
_channelType(t._channelType)
{
	t._desc.clear();
	t._key = 0;
	t._name.clear();
	t._offset = 0;
	t._type = 0;
	t._unit.clear();
}

CDataChannel& CDataChannel::operator = (CDataChannel&& t)
{
	_name = t._name;
	_desc = t._desc;
	_unit = t._unit;
	_key = t._key;
	_offset = t._offset;
	_type = t._type;
	_channelType = t._channelType;

	t._desc.clear();
	t._key = 0;
	t._name.clear();
	t._offset = 0;
	t._type = 0;
	t._unit.clear();
	t._channelType = ChannelType_Unknown;

	return *this;
}

const char* CDataChannel::get_Name()
{
	return _name.c_str();
}

const char* CDataChannel::get_Unit()
{
	return _unit.c_str();
}

const char* CDataChannel::get_Description()
{
	return _desc.c_str();
}

int CDataChannel::get_Type()
{
	return _type;
}

int CDataChannel::get_Key()
{
	return _key;
}

int CDataChannel::get_Offset()
{
	return _offset;
}

ChannelType CDataChannel::get_ChannelType()
{
	return _channelType;
}

#pragma endregion

SampleValue CDataChannel::GetSampleData(IGenericLogger& logger, DataSample& s, CDataChannel& c)
{
	SampleValue val;

	if (c.get_ChannelType() == ChannelType_File) {
		switch (c.get_Type())
		{
		case irsdk_int:
		{
			int i = 0;
			memcpy(&i, &s[c.get_Offset()], sizeof(int));
			val = SampleValue(i);
			break;
		}
		case irsdk_float:
		{
			float f = (float)*(&s[c.get_Offset()]);
			memcpy(&f, &s[c.get_Offset()], sizeof(float));
			val = SampleValue(f);
			break;
		}
		case irsdk_double:
		{
			double d = (double)*(&s[c.get_Offset()]);
			memcpy(&d, &s[c.get_Offset()], sizeof(double));
			val = SampleValue(d);
			break;
		}
		case irsdk_char:
		{
			char _c = (char)*(&s[c.get_Offset()]);
			memcpy(&c, &s[c.get_Offset()], sizeof(char));
			val = SampleValue(_c);
			break;
		}
		case irsdk_bool:
		{
			if (sizeof(bool) == 1)
			{
				BYTE b = (BYTE)*(&s[c.get_Offset()]);
				memcpy(&b, &s[c.get_Offset()], sizeof(BYTE));
				if (b)
					val = SampleValue((bool)true);
				else
					val = SampleValue((bool)false);
			}
			else
			{
				DWORD b = (DWORD)*(&s[c.get_Offset()]);
				memcpy(&b, &s[c.get_Offset()], sizeof(BYTE));
				if (b)
					val = SampleValue((bool)true);
				else
					val = SampleValue((bool)false);
			}
			break;
		}
		case irsdk_bitField:
		{
			DWORD dw = (DWORD)*(&s[c.get_Offset()]);
			memcpy(&dw, &s[c.get_Offset()], sizeof(DWORD));
			val = SampleValue(dw);
			break;
		}
		default:
			break;
		}
	}
	else if (c.get_ChannelType() == ChannelType_Math) {

		float v = 0;

		if (c.get_Name() == std::string(CH_SP_SPEED_KPH))
		{
			CDataChannel& ch = logger.GetChannel(std::string("Speed"));
			SampleValue val = CDataChannel::GetSampleData(s, ch);
			v = val.get_value<float>(); // ((CChannel<float>*)exportedChannels[channelsMap[specialChannelsMap[CH_SP_SPEED_KPH]]])->GetChannelData(i);
			v *= 3.6f;
			return SampleValue(v);
		}
		else if (c.get_Name() == std::string(CH_SP_SPEED_MPH))
		{
			CDataChannel& ch = logger.GetChannel(std::string("Speed"));
			SampleValue val = CDataChannel::GetSampleData(s, ch);
			v = val.get_value<float>();
			v *= 2.23694f;
			return SampleValue(v);
		}
		else if (c.get_Name() == std::string(CH_SP_LATG))
		{
			CDataChannel& ch = logger.GetChannel(std::string("LatAccel"));
			SampleValue val = CDataChannel::GetSampleData(s, ch);
			v = val.get_value<float>();
			v /= 9.81f;
			return SampleValue(v);
		}
		else if (c.get_Name() == std::string(CH_SP_LONGG))
		{
			CDataChannel& ch = logger.GetChannel(std::string("LongAccel"));
			SampleValue val = CDataChannel::GetSampleData(s, ch);
			v = val.get_value<float>();
			v /= 9.81f;
			return SampleValue(v);
		}
	}

	return std::move(val);
}

SampleValue CDataChannel::GetSampleData(DataSample& s, CDataChannel& c)
{
	SampleValue val;

	if (c.get_ChannelType() == ChannelType_File) {
		switch (c.get_Type())
		{
		case irsdk_int:
		{
			int i = 0;
			memcpy(&i, &s[c.get_Offset()], sizeof(int));
			val = SampleValue(i);
			break;
		}
		case irsdk_float:
		{
			float f = (float)*(&s[c.get_Offset()]);
			memcpy(&f, &s[c.get_Offset()], sizeof(float));
			val = SampleValue(f);
			break;
		}
		case irsdk_double:
		{
			double d = (double)*(&s[c.get_Offset()]);
			memcpy(&d, &s[c.get_Offset()], sizeof(double));
			val = SampleValue(d);
			break;
		}
		case irsdk_char:
		{
			char _c = (char)*(&s[c.get_Offset()]);
			memcpy(&c, &s[c.get_Offset()], sizeof(char));
			val = SampleValue(_c);
			break;
		}
		case irsdk_bool:
		{
			if (sizeof(bool) == 1)
			{
				BYTE b = (BYTE)*(&s[c.get_Offset()]);
				memcpy(&b, &s[c.get_Offset()], sizeof(BYTE));
				if (b)
					val = SampleValue((bool)true);
				else
					val = SampleValue((bool)false);
			}
			else
			{
				DWORD b = (DWORD)*(&s[c.get_Offset()]);
				memcpy(&b, &s[c.get_Offset()], sizeof(BYTE));
				if (b)
					val = SampleValue((bool)true);
				else
					val = SampleValue((bool)false);
			}
			break;
		}
		case irsdk_bitField:
		{
			DWORD dw = (DWORD)*(&s[c.get_Offset()]);
			memcpy(&dw, &s[c.get_Offset()], sizeof(DWORD));
			val = SampleValue(dw);
			break;
		}
		default:
			break;
		}
	}
	else {
		throw std::exception("Invalid data channel specified");
	}
	return std::move(val);
}


//bool CBaseChannel::GetSample(DataSample&)
//{
//	return false;
//}
//
//bool CBaseChannel::GetSample(DataSample&, size_t)
//{
//	return false;
//}


IDataLogFile* __get_inst(std::wstring file)
{
	IDataLogFile* p = new CIracingBinaryTelemetryFile(file);

	return p;
}


ILiveDataLogger* __get_live(ILiveLoggingCallback& callback)
{
	ILiveDataLogger* p = new CLiveDataLogger(callback);
	return p;
}

void __release_inst(void* inst)
{
	if (inst != nullptr)
	{
		delete inst;
		inst = nullptr;
	}
}

#pragma region // Interface Instance Factory class implementation
IDataLogFile* LoggerInstanceFactory::GetFileLogInstance(const wchar_t* fileName)
{
	if (fileName != nullptr)
	{
		return new CIracingBinaryTelemetryFile(std::wstring(fileName));
	}

	return nullptr;
}

void LoggerInstanceFactory::ReleaseFileLogInstance(IDataLogFile* inst)
{
	if (inst != nullptr)
	{
		CIracingBinaryTelemetryFile* p=static_cast<CIracingBinaryTelemetryFile*>(inst);
		delete p;
		p = nullptr;
		inst = nullptr;
	}
}

libOGA::LiveLogs::ILiveDataLogger* LoggerInstanceFactory::GetLiveLoggerInstance(libOGA::LiveLogs::ILiveLoggingCallback& cb)
{
	return new CLiveDataLogger(cb);
}

void LoggerInstanceFactory::ReleaseLiveLoggerInstance(libOGA::LiveLogs::ILiveDataLogger* inst)
{
	if (inst != nullptr) {
		CLiveDataLogger* p = static_cast<CLiveDataLogger*>(inst);
		delete p;
		p = nullptr;
		inst = nullptr;
	}
}

#pragma endregion

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID p)
{
	return TRUE;
}