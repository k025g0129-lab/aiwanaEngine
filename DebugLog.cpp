#include "DebugLog.h"

#include <filesystem>
#include <chrono>
#include <Windows.h>

std::ofstream DebugLog::logStream_;

void DebugLog::Initialize(){
	//デバックログ
	std::filesystem::create_directory("logs");

	//後に関数化予定
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::time_point < std::chrono::system_clock, std::chrono::seconds > nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	std::chrono::zoned_time localTime{ std::chrono::current_zone(),nowSeconds };


	/*std::string dateString = std::format("{:%Y%m%d_%H%M%S}", localTime);
	std::string logFilePath = std::string("logs/") + dateString + ".log";
	std::ofstream logStream(logFilePath);*/

	std::string fileName = std::format("logs/{:%Y%m%d_%H%M%S}.log",localTime);

	logStream_.open(fileName);

}

void DebugLog::Shutdown(){
	if (logStream_.is_open()) {
		logStream_.close();
	}
}
