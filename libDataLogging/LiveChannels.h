#pragma once

#include <string>
#include <map>
#include <mutex>

#include "irsdk_defines.h"

namespace libDataLogging{
	namespace LiveLogging {


#pragma region "CLiveChannel"
		// This class defines all properties of a live logger data channel
		class CLiveChannel
		{
		public:

			CLiveChannel(std::string name,
				std::string unit,
				std::string desc,
				int key,
				int type,
				int offset)
			{
				_name = name;
				_unit = unit;
				_desc = desc;
				_key = key;
				_type = type;
				_offset = offset;
			}
			~CLiveChannel() {}

			CLiveChannel(CLiveChannel&& t) :
				_name(t._name),
				_desc(t._desc),
				_unit(t._unit),
				_type(t._type),
				_key(t._key),
				_offset(t._offset)
			{
				t._desc.clear();
				t._key = 0;
				t._name.clear();
				t._offset = 0;
				t._type = 0;
				t._unit.clear();
			}

			CLiveChannel& operator=(CLiveChannel&& t)
			{
				_name = t._name;
				_desc = t._desc;
				_unit = t._unit;
				_key = t._key;
				_offset = t._offset;
				_type = t._type;

				t._desc.clear();
				t._key = 0;
				t._name.clear();
				t._offset = 0;
				t._type = 0;
				t._unit.clear();

				return *this;
			}

			std::string& get_Name()
			{
				return _name;
			}

			std::string get_Unit()
			{
				return _unit;
			}

			std::string get_Description()
			{
				return _desc;
			}

			int get_Type()
			{
				return _type;
			}

			int get_Key()
			{
				return _key;
			}

			int get_Offset()
			{
				return _offset;
			}

		protected:

			std::string _name; // channel name
			std::string _desc; // channel description
			std::string _unit; // channel units
			int _type; // channels data type
			int _key; // channels index in channel map

			int _offset; // offset of data inside the sample buffer
		};
		typedef std::map<std::string, CLiveChannel> ChannelsMap; // mapping of channel descriptor to data buffer index; the value is found at the int index in the data buffer
#pragma endregion

#pragma region "SampleValue"
		// This class represents a unique and unified data structure for dynamic (know at runtime)
		// sample data value types, so there is a know data type available at compile time.
		class SampleValue
		{
			int _iVal;
			float _fVal;
			double _dVal;
			char _cVal;
			bool _bVal;
			unsigned long _uVal; // bitmask

			int _type;

		public:
			SampleValue() :
				_iVal(0),
				_fVal(0),
				_dVal(0),
				_cVal(0),
				_bVal(0),
				_uVal(0),
				_type(-1)
			{}

			SampleValue(int val) :
				_iVal(val),
				_fVal(0),
				_dVal(0),
				_cVal(0),
				_bVal(0),
				_uVal(0),
				_type(iR_sdk::irsdk_int)
			{
			}

			SampleValue(float val) :
				_iVal(0),
				_fVal(val),
				_dVal(0),
				_cVal(0),
				_bVal(0),
				_uVal(0),
				_type(iR_sdk::irsdk_float)
			{
			}

			SampleValue(double val) :
				_iVal(0),
				_fVal(0),
				_dVal(val),
				_cVal(0),
				_bVal(0),
				_uVal(0),
				_type(iR_sdk::irsdk_double)
			{
			}

			SampleValue(char val) :
				_iVal(0),
				_fVal(0),
				_dVal(0),
				_cVal(val),
				_bVal(0),
				_uVal(0),
				_type(iR_sdk::irsdk_char)
			{
			}

			SampleValue(bool val) :
				_iVal(0),
				_fVal(0),
				_dVal(0),
				_cVal(0),
				_bVal(val),
				_uVal(0),
				_type(iR_sdk::irsdk_bool)
			{
			}

			SampleValue(unsigned long val) :
				_iVal(0),
				_fVal(0),
				_dVal(0),
				_cVal(0),
				_bVal(0),
				_uVal(val),
				_type(iR_sdk::irsdk_bitField)
			{
			}

			int type()
			{
				return _type;
			}

			template<typename T>
			T get_value() {}

			template<>
			int get_value<int>()
			{
				return _iVal;
			}

			template<>
			float get_value<float>()
			{
				return _fVal;
			}

			template<>
			double get_value<double>()
			{
				return _dVal;
			}

			template<>
			char get_value<char>()
			{
				return _cVal;
			}

			template<>
			bool get_value<bool>()
			{
				return _bVal;
			}

			template<>
			unsigned long get_value<unsigned long>()
			{
				return _uVal;
			}
		};
#pragma endregion

	}
}