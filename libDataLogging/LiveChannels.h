#pragma once

#include <string>
#include <map>
#include <mutex>

#include "irsdk_defines.h"

namespace libDataLogging{
	namespace LiveLogging {



		// channel descriptor 
		// this datastructure is the same for all channels and describe the actual data channel
		struct LiveChannelDescriptor
		{
			LiveChannelDescriptor() :
				name(),
				desc(),
				unit(),
				type(),
				key(key)
			{}

			LiveChannelDescriptor(const std::string name, const std::string desc, const std::string unit, int type, int key) :
				name(name),
				desc(desc),
				unit(unit),
				type(type),
				key(key)
			{
			}

			LiveChannelDescriptor& operator= (const LiveChannelDescriptor& t)
			{
				name = std::move(t.name);
				unit = std::move(t.unit);
				desc = std::move(t.desc);
				type = std::move(t.type);
				key = std::move(t.key);

				return *this;
			}

			const std::string& Name() const
			{
				return name;
			}

			const std::string& Description() const
			{
				return desc;
			}

			const std::string& Unit() const
			{
				return unit;
			}

			const int Type() const
			{
				return type;
			}

			const int Key() const
			{
				return key;
			}

		private:
			std::string name;
			std::string desc;
			std::string unit;
			int type;
			int key; // index key in data map
		};
		typedef std::map<std::string, LiveChannelDescriptor> ChannelsMap; // mapping of channel descriptor to data buffer index; the value is found at the int index in the data buffer


		//class BaseLiveChannel
		//{
		//public:
		//	BaseLiveChannel() {}
		//	~BaseLiveChannel() {}

		//protected:

		//};

		//template<typename T>
		//class LiveChannel : public BaseLiveChannel
		//{
		//public:
		//	LiveChannel(std::mutex& mtx)
		//	{
		//		_mtx = mtx;
		//	}


		//	LiveChannel(const T& val, std::mutex& mtx) : BaseLiveChannel()
		//	{
		//		_mtx.lock();
		//		_value = val;
		//		_mtx.unlock();
		//	}

		//	LiveChannel(const T&& val)
		//	{
		//	}

		//	~LiveChannel() {}

		//	LiveChannel& operator=(const T& val)
		//	{
		//		_mtx.lock();
		//		_value = val;
		//		_mtx.unlock();

		//		return *this;
		//	}

		//	T Value() const
		//	{				
		//		return _value;
		//	}

		//protected:

		//	T _value;
		//	std::mutex _mtx;
		//};


		struct LiveChannel
		{
			int type;
			union Value {
				int _int;
				char _char;
				float _float;
				double _double;
				int _bool;
			}SampleData;
		};
	}
}