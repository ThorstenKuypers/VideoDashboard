#pragma once

#include <windows.h>
#include <vector>
#include <string>

#include "libDataLogging.h"
//#include "irsdk_defines.h"


namespace libDataLogging {
	namespace DataLogger {

		using namespace iR_sdk;

		template<typename T>
		class CChannel : CBaseChannel
		{
		public:

			CChannel() :CBaseChannel()
			{
				int x = 0;
				samples = std::vector<T>();
			}

			CChannel(char* name, char* desc, char* unit, int count) : CBaseChannel()
			{
				if (name != NULL) {
					_name = std::string(name);
				}
				if (desc != NULL) {
					_description = string(desc);
				}
				if (unit != NULL) {
					_unit = string(unit);
				}

				this->sampleCount = count;
				this->idx = 0;

				samples = std::vector<T>(count);

				setChannelType();
			}

			CChannel(char* name, int samplesCount) : CBaseChannel()
			{
				if (name != NULL)
					_name = string(name);

				this->sampleCount = samplesCount;
				this->idx = 0;

				samples = std::vector<T>(samplesCount);

				setChannelType();
			}

			CChannel<T> operator[] (CChannel<T>&)
			{
				int x = 0;
				return *this;
			}

			~CChannel()
			{
			};

			void AddSample(T value)
			{
				if ((idx >= 0) && (idx < sampleCount))
				{
					samples[idx] = value;
					idx++;
				}
			}

			T GetChannelData(unsigned int index)
			{
				if ((index >= 0) && (index < samples.size()))
				{
					return samples[index];
				}

				return 0;
			}

			const char* GetName()
			{
				return (const char*)_name.c_str();
			}

			const char* GetDescription()
			{
				return &_description[0];
			}

			void SetDescription(char* desc)
			{
				if (desc != NULL)
				{
					_description = desc;
				}
			}

			char* GetUnits()
			{
				return &_unit[0];
			}

			void SetUnits(char* units)
			{
				if (units != NULL) {
					_unit = string(units);
				}
			}

			int GetSamplesCount()
			{
				return samples.size();
			}

			ChannelType GetChannelDataType()
			{
				return (ChannelType)type;
			}

		private:
			std::vector<T> samples;

			void setChannelType()
			{
				if (typeid(T) == typeid(float))
					type = (int)irsdk_float;
				else if (typeid(T) == typeid(double))
					type = (int)irsdk_double;
				else if (typeid(T) == typeid(int))
					type = (int)irsdk_int;
				else if (typeid(T) == typeid(char))
					type = (int)irsdk_char;
				else if (typeid(T) == typeid(bool))
					type = (int)irsdk_bool;
			}
		};

	}

}