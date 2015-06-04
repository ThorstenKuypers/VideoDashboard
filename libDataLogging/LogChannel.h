#pragma once

namespace libDataLogging {
	namespace DataLogWrapper {

using namespace System;
using namespace System::Collections::Generic;

#pragma managed
	
		public ref class BaseLogChannel
		{
			protected:
				String^ _name;
				String^ _description;
				String^ _unit;

				int _sampleCount;
				int idx;

		};

		generic<typename T> public ref class LogChannel : BaseLogChannel
		{
			public:

				LogChannel()
				{
				}

				LogChannel(String^ name, String^ desc, String^ unit, int samplesCnt)
				{
					_name =name;
					_description =desc;
					_unit =unit;
					_sampleCount =samplesCnt;

					idx =0;

					_samples =gcnew cli::array<T>(_sampleCount);
				}

				virtual ~LogChannel()
				{
					if (_samples !=nullptr) {

						delete _samples;
						_samples =nullptr;
					}
				}

				T GetSampleData(int index)
				{
					if (index >= 0 && index < _samples->Length)
						return _samples[index];

					return (T)0;
				}

				property System::String^ Name
				{
					System::String^ get() { return _name; }
				}

				property System::String^ Description
				{
					System::String^ get() { return _description; }
				}

				property System::String^ Unit
				{
					System::String^ get() { return _unit; }
				}

				property cli::array<T>^ Samples
				{
					cli::array<T>^ get() { return _samples; }
				}

			private:

				cli::array<T>^ _samples;

		};
}
}