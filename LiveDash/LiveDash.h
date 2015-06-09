#pragma once

#include "libLDF.h"
#include "libDataLogging.h"
#include "LiveDataLogger.h"

#include "config.h"

using namespace libDataLogging;
using namespace libDataLogging::LiveDataLogger;
using namespace libLDF;


void LoggerConnect(EventArgs& e);
void LoggerDisconnect(EventArgs& e);
void LoggerUpdate(SampleDataUpdateEventArgs& e);
