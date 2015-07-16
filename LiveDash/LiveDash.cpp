#include "Plugin.h"
#include "LiveDash.h"

using namespace libOGA;
using namespace libOGA::LiveLogs;
using namespace libLDF;


CLiveDashboard::CLiveDashboard() :
_dash(nullptr),
_logger(nullptr),
_simConnected(false),
_cfg(),
mtx()
{
	_logger = __get_live(*this);
	if (_logger == nullptr)
		throw std::exception("error during live logger initialization!");

	_dash = get_LDF();
	if (_dash == nullptr) {
		__release_inst(_logger);

		throw std::exception("error during LDF initialization!");
	}

	_cfg.dash = _dash;
	_cfg.logger = _logger;

}

CLiveDashboard::~CLiveDashboard()
{
	if (_logger != nullptr){
		__release_inst(_logger);
		_logger = nullptr;
	}

	if (_dash != nullptr)
	{
		release_LDF(_dash);
		_dash = nullptr;
	}
}

void CLiveDashboard::Init()
{

}

void CLiveDashboard::OnStartStream()
{
	_logger->Start();
	int x = 0;
}

void CLiveDashboard::OnStopStream()
{
	_logger->Stop();
	int x = 0;
}

void CLiveDashboard::OnStartStreaming()
{
	int x = 0;
}

void CLiveDashboard::OnStopStreaming()
{
	int x = 0;
}

void CLiveDashboard::SimConnected()
{
	_simConnected = true;
}

void CLiveDashboard::SimDataUpdate(libOGA::DataSample& sample)
{
	std::lock_guard<std::mutex> lock{ mtx };
	
	_latestSample = sample;
}

void CLiveDashboard::SimDisconnected()
{
	_simConnected = false;
}

