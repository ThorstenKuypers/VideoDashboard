#pragma once

#include <functional>
#include <vector>
#include <thread>
#include <memory>

#include "LiveChannels.h"

namespace libDataLogging {
	namespace LiveLogging {

#pragma region "Event base implementations"
		// base object for event specific arguments
		struct EventArgs
		{
		public:
			EventArgs() {}
			virtual ~EventArgs() {}
		};

		struct SampleDataUpdateEventArgs : public EventArgs
		{
		public:
			SampleDataUpdateEventArgs() {}
			virtual ~SampleDataUpdateEventArgs() {}
		};

		typedef std::function <void(EventArgs)> Event;
		typedef std::function<void(SampleDataUpdateEventArgs)> DataUpdateEvent;

		// creates a delegate from a class memebr function
#define MAKE_DELEGATE(f, c) std::bind(&f, c, std::placeholders::_1)

		// creates a delegate from a C function (ptr) 
#define MAKE_DELEGATE_FN(f) &f


		// base EventHandler class implementation
		template<class function, class Args>
		class EventHandler
		{
		public:
			virtual void operator() (Args& e)
			{
				for (auto ev : _events) {
					std::thread t = std::thread(ev, e);
					t.detach();
				}
			}

			void add(const function& fn)
			{
				_events.push_back(fn);
			}

			void remove(function& fn)
			{
				//for (DWORD i = 0; i < _events.size(); i++) {

				//	auto f = _events[i];
				//	if (f == fn)
				//		_events.erase(_events.begin() + i);
				//}
			}

		protected:
			std::vector<function> _events;
		};

#pragma endregion

#pragma region "Inherited EventHandler class implementaions"

		class SimConnectedEventHandler : public EventHandler < Event, EventArgs >
		{
		public:

			SimConnectedEventHandler& operator=(Event& tmp) = delete;
			SimConnectedEventHandler& operator=(Event&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}

			SimConnectedEventHandler& operator+=(Event& tmp) = delete;
			SimConnectedEventHandler& operator+=(Event&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}
		};


		class SimDisconnectedEventHandler : public EventHandler < Event, EventArgs >
		{
		public:

			SimDisconnectedEventHandler& operator=(Event& tmp) = delete;
			SimDisconnectedEventHandler& operator=(Event&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}

			void operator-(Event& tmp) = delete;
			void operator-(Event&& tmp) {
				this->remove(tmp);
				tmp = nullptr;
			}

			void operator-=(Event& tmp) = delete;
			void operator-=(Event&& tmp) {
				this->remove(tmp);
				tmp = nullptr;
			}

			SimDisconnectedEventHandler& operator+=(Event& tmp) = delete;
			SimDisconnectedEventHandler& operator+=(Event&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}
		};

		class SampleDataUpdateEventHandler : public EventHandler < DataUpdateEvent, SampleDataUpdateEventArgs >
		{
		public:
			virtual void operator() (SampleDataUpdateEventArgs& e) override
			{
				for (auto ev : _events) {
					std::thread t = std::thread(ev, e);
					t.detach();
				}
			}

			SampleDataUpdateEventHandler& operator=(DataUpdateEvent& tmp) = delete;
			SampleDataUpdateEventHandler& operator=(DataUpdateEvent&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}

			SampleDataUpdateEventHandler& operator+=(DataUpdateEvent& tmp) = delete;
			SampleDataUpdateEventHandler& operator+=(DataUpdateEvent&& tmp) {
				this->add(tmp);
				tmp = nullptr;

				return *this;
			}

		};

#pragma endregion
	}
}