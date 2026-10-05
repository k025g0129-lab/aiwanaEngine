#pragma once
#include "DebugLog.h"

#define LOG(...) DebugLog::Log(__FILE__, __LINE__, __VA_ARGS__)