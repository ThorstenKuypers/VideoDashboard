#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//	TelemetryVideoOverlay
//	----------------------
//	Project: TVO -- Main GUI implementation
//
//	Copyright 2011 Thorsten Kuypers
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You should have obtained a copy of the License with this Software. If not,
//	you may obtain a copy of the License at
//
//				http://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
// * version history:
//   -----------------
//
//	0.4 - first public release
//
///////////////////////////////////////////////////////////////////////////////

#include "libDataLogging.h"
#include "ErrEx.h"
#include "LogChannel.h"


namespace libDataLogging {
	namespace DataLogWrapper {
#pragma managed

		using namespace System;
		using namespace System::ComponentModel;
		using namespace System::Runtime::InteropServices;
		using namespace System::Collections::Generic;

		using namespace libDataLogging::DataLogger;

		[StructLayout(LayoutKind::Sequential)]
		public ref struct DataLogSample {

			double time;			// SessionTime
			float corrSpeed;		// 
			int gear;				// Gear
			float engineRpm;		// Rpm
			float throttlePos;		// Throttle
			float brakePos;			// Brake
			float steerAngle;		// SteeringAngle
			int LapNo;				// CurrentLapNo
			float groundSpeed;		// Speed
			float latG;				// LatG
			float longG;			// LongG
			float lapPct;			// LapPercent

		};

		[StructLayout(LayoutKind::Sequential)]
		public ref struct LapInfo {

			int lapNo;	// current lap no
			int sampleIdx;	// index in sample buffer |_ start of lap
			int sampleCnt;		// data samples count
			double time;	// session time index	  |
			float lapTime;
		};


		[StructLayout(LayoutKind::Sequential)]
		public ref struct FastestLapInfo {

			// fastest lap in session (from SessionInfoString)
			float fastestLapTimeInSession;
			float fastestLapTimeInStint;
			int fastestLapSession;
			int fastestLapStint;

		};

		public enum struct Channels {

			SessionTime = 0,
			Speed,
			Gear,
			Rpm,
			Throttle,
			Brake,
			SteeringWheelAngle,
			Lap,
			LatAccel,
			LongAccel,
			LapDistPct
		};

		public enum struct ChannelDataTypes {

			ChannelType_float = 0,
			ChannelType_int
		};

		public ref class CDataLog
		{
		private:
			wchar_t* m_logfile;

			cli::array<DataLogSample^>^ m_sampleList;
			cli::array<LapInfo^>^ m_lapInfoBuf;
			List<LapInfo^>^ m_sessionLapsList;
			//List<BaseLogChannel^>^ m_channelsList;

			int m_sessionLapCnt;
			int m_sessionCurLapIdx;

			int m_sampleListStartIdx;
			int m_sampleListStopIdx;

			IDataLogging* m_dataLogger;

			void InitSampleList()
			{
				try
				{
					//m_channelsList =gcnew List<BaseLogChannel^>();

					//std::vector<CBaseChannel*> chs =m_dataLogger->GetExportedChannels();
					//for (unsigned int i =0; i < chs.size(); i++) {

					//	if (chs[i]->GetType() ==ChannelType::irsdk_float) {

					//		m_channelsList->Add(gcnew LogChannel<float>(gcnew String(chs[i]->GetName().c_str()),
					//													gcnew String(chs[i]->GetDescription().c_str()),
					//													gcnew String(chs[i]->GetUnit().c_str()),
					//													m_dataLogger->GetSampleCount()));
					//	}
					//}

					//int x =0;
				}
				catch (CErrEx* ex)
				{
					Exception^ ix = gcnew Exception(gcnew String(ex->logMsg));
					Exception^ e = gcnew Exception(gcnew String(ex->msg), ix);

					delete ex;
					throw e;
				}
			}

			void InitLapTimigInfo()
			{
				int cnt = 0;
				int lapcnt = 0;
				lapcnt = this->m_dataLogger->GetSessionLapCount();
				cli::array<LapInfo^>^ lib = gcnew cli::array<LapInfo^>(lapcnt);

				for (int i = 0; i < lapcnt; i++) {

					LapInfo^ li = gcnew LapInfo();

					IntPtr ptr = IntPtr((void*)this->m_dataLogger->GetLapInfo(i));
					Marshal::PtrToStructure(ptr, li);
					if (li->lapTime > 0) {
						lib[i] = li;
						cnt++;
					}
				}

				this->m_sessionLapCnt = cnt;
				this->m_lapInfoBuf = gcnew cli::array<LapInfo^>(cnt);
				for (int i = 0; i < cnt; i++) {
					if (lib[i] != nullptr) {
						this->m_lapInfoBuf[i] = lib[i];
					}
				}
				if (lib) {
					delete lib;
					lib = nullptr;
				}
			}

		public:

			property int RedlineRPM
			{
				int get()
				{
					return m_dataLogger->RedlineRPM();
				}
			}

			property int ShiftRPM
			{
				int get()
				{
					return m_dataLogger->ShiftLightRPM();
				}
			}


			CDataLog(String^ fileName)
			{
				IntPtr p = Marshal::StringToHGlobalUni(fileName);

				try
				{
					wchar_t* t = (wchar_t*)p.ToPointer();
					this->m_dataLogger = init_instance(t);
					if (!this->m_dataLogger) {

						String^ msg = gcnew String("Error in libdl_init_instance()");
						throw msg;
					}
					InitSampleList();
					InitLapTimigInfo();
				}
				catch (CErrEx* ex)
				{
					Exception^ ix = gcnew Exception(gcnew String(ex->logMsg));
					Exception^ e = gcnew Exception(gcnew String(ex->msg), ix);

					delete ex;
					throw e;
				}

				Marshal::FreeHGlobal(p);

				//XXX:
			}

			~CDataLog(void)
			{
				release_instance(m_dataLogger);
				m_dataLogger = 0;
				this->!CDataLog();
			}

			!CDataLog(void)
			{
				if (this->m_lapInfoBuf != nullptr) {
					for (int i = 0; i < m_lapInfoBuf->Length; i++) {
						if (m_lapInfoBuf[i]) {
							delete m_lapInfoBuf[i];
							m_lapInfoBuf[i] = nullptr;
						}
					}
					delete this->m_lapInfoBuf;
					this->m_lapInfoBuf = nullptr;
				}

				if (m_sampleList != nullptr) {
					for (int i = 0; i < m_sampleList->Length; i++)
					{
						if (m_sampleList[i] != nullptr)
						{
							delete m_sampleList[i];
							m_sampleList[i] = nullptr;
						}
					}
					delete m_sampleList;
					m_sampleList = nullptr;
				}
			}

			int GetLapIndex(int LapNumber)
			{
				int i;

				for (i = 0; i < m_lapInfoBuf->Length; i++)
				{
					if (m_lapInfoBuf[i]->lapNo == LapNumber)
						break;
				}

				return i + 1;
			}

			String^ GetChannelDescription(String^ channel)
			{
				IntPtr ptr = Marshal::StringToHGlobalAnsi(channel);
				std::string _ch((char*)ptr.ToPointer());

				CBaseChannel c = m_dataLogger->GetChannel(_ch);

				String^ desc = gcnew String(c.GetDescription().c_str());

				Marshal::FreeHGlobal(ptr);

				return desc;
			}

			String^ GetChannelUnit(String^ channel)
			{
				IntPtr ptr = Marshal::StringToHGlobalAnsi(channel);
				std::string _ch((char*)ptr.ToPointer());

				CBaseChannel c = m_dataLogger->GetChannel(_ch);

				String^ unit = gcnew String(c.GetUnit().c_str());

				Marshal::FreeHGlobal(ptr);

				return unit;
			}

			DataLogSample^ GetSample(int index)
			{
				try {
					double vd = 0;
					int vi = 0;
					float vf = 0;

					DataLogSample^ s = gcnew DataLogSample();
					m_dataLogger->GetChannelData(string("SessionTime"), index, &vd);
					s->time = vd;

					m_dataLogger->GetChannelData(string("Speed"), index, &vf);
					s->groundSpeed = vf;

					m_dataLogger->GetChannelData(string("Gear"), index, &vi);
					s->gear = vi;

					m_dataLogger->GetChannelData(string("RPM"), index, &vf);
					s->engineRpm = vf;

					m_dataLogger->GetChannelData(string("Throttle"), index, &vf);
					s->throttlePos = vf;

					m_dataLogger->GetChannelData(string("SteeringWheelAngle"), index, &vf);
					s->steerAngle = vf;

					m_dataLogger->GetChannelData(string("Lap"), index, &vi);
					s->LapNo = vi;

					m_dataLogger->GetChannelData(string("LapDistPct"), index, &vf);
					s->lapPct = vf;

					m_dataLogger->GetChannelData(string("LatAccel"), index, &vf);
					s->latG = vf;

					m_dataLogger->GetChannelData(string("LongAccel"), index, &vf);
					s->longG = vf;

					m_dataLogger->GetChannelData(string("Brake"), index, &vf);
					s->brakePos = vf;

					return s;
				}
				catch (Exception^ ex)
				{
					throw; // ex;
				}

				return nullptr;
			}

			Object^ GetChannelData(String^ channel, int index)
			{
				Object^ o = nullptr;
				char* ch = (char*)Marshal::StringToHGlobalAnsi(channel).ToPointer();
				string str = string(ch);

				double vd = 0;
				float vf = 0;
				int vi = 0;
				char vc = 0;
				bool vb = false;

				CBaseChannel& c = m_dataLogger->GetChannel(string(ch));
				if (c.GetType() == ChannelType::irsdk_double) {

					m_dataLogger->GetChannelData(std::string(ch), index, (double*)&vd);
					o = (double)vd;
				}
				else if (c.GetType() == ChannelType::irsdk_float) {

					m_dataLogger->GetChannelData(std::string(ch), index, (float*)&vf);
					o = (float)vf;
				}
				else if (c.GetType() == ChannelType::irsdk_int) {

					m_dataLogger->GetChannelData(std::string(ch), index, (int*)&vi);
					o = (int)vi;
				}
				else if (c.GetType() == ChannelType::irsdk_char) {

					m_dataLogger->GetChannelData(std::string(ch), index, (char*)&vc);
					o = (char)vc;
				}
				else if (c.GetType() == ChannelType::irsdk_bool) {

					m_dataLogger->GetChannelData(std::string(ch), index, (bool*)&vb);
					o = (bool)vb;
				}

				Marshal::FreeHGlobal(IntPtr(ch));
				return o;
			}

			LapInfo^ GetLapInfo(int lap)
			{
				if (!(lap > 0 && lap <= this->m_sessionLapCnt))
					return nullptr;

				return this->m_lapInfoBuf[lap - 1];
			}

			LapInfo^ GetFastestLap()
			{
				float fast = 0, tmp = 0;
				int idx = 0;

				for (int i = 0; i < m_lapInfoBuf->Length; i++)
				{
					if ((i > 0) && (i < (m_lapInfoBuf->Length - 1))) {
						tmp = m_lapInfoBuf[i]->lapTime;
						if (fast == 0) {
							fast = tmp;
							idx = i;
						}

						if (tmp < fast && tmp > 0) {
							fast = tmp;
							idx = i;
						}
					}
				}

				return m_lapInfoBuf[idx];
			}

			property int SampleCount
			{
				int get()
				{
					return this->m_dataLogger->GetSampleCount();
				}
			}

			property int TickRate
			{
				int get()
				{
					return this->m_dataLogger->GetSampleTickRate();
				}
			}

			property int SessionLapCount
			{
				int get()
				{
					return this->m_sessionLapCnt;
				}
			}

			property cli::array<LapInfo^>^ LapsArray
			{
				cli::array<LapInfo^>^ get()
				{
					return m_lapInfoBuf;
				}
			}

			IntPtr getDataLoggerObjPtr()
			{
				if (this->m_dataLogger)
					return IntPtr((void*)this->m_dataLogger);
				return IntPtr(0);
			}

			property cli::array<String^>^ ExportedChannelNames
			{
				cli::array<String^>^ get()
				{
					std::vector<string> expChannels = m_dataLogger->GetExportedChannelNames();

					cli::array<String^>^ a = gcnew cli::array<String^>(expChannels.size());
					for (unsigned int i = 0; i < expChannels.size(); i++) {

						a[i] = gcnew String((char*)expChannels[i].c_str());
					}
					return a;
				}

			}

			property cli::array<String^>^ SpecialChannelNames
			{
				cli::array<String^>^ get()
				{
					std::vector<std::string> vc;
					m_dataLogger->GetSpecialChannelNames(&vc);

					cli::array<String^>^ a = gcnew cli::array<String^>(vc.size());
					int i = 0;
					for (auto c : vc) {

						a[i] = gcnew String((char*)c.c_str());
						i++;
					}
					return a;
				}
			}

			//property cli::array<String^>^ TEST
			//{
			//	cli::array<String^>^ get()
			//	{
			//		std::vector<CBaseChannel*> ch = m_dataLogger->GetExportedChannels();

			//		cli::array<String^>^ a = gcnew cli::array<String^>(ch.size());
			//		for (size_t i = 0; i < ch.size(); i++)
			//		{
			//			a[i] = gcnew String((char*)ch[i]->GetName().c_str());
			//		}

			//		return a;
			//	}
			//}
		};

	}
}