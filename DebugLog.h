#pragma once
#include <fstream>
#include <string>
#include <format>

#include <filesystem>
class DebugLog{
public:

	static void Initialize();
	static void Shutdown();

	template<class... Args>
	static void Log(const char* file, int line, std::string_view fmt, Args&&... args);

private:
	static std::ofstream logStream_;
};

template<class ...Args>
inline void DebugLog::Log(const char* file, int line, std::string_view fmt, Args && ...args){

    std::string message =
        std::vformat(fmt, std::make_format_args(args...));

    std::string fileName = std::filesystem::path(file).filename().string();

    std::string finalMessage =
        std::format("[{}:{}] {}", fileName, line, message);

    if (logStream_.is_open()) {
        logStream_ << finalMessage << std::endl;
    }

    OutputDebugStringA((finalMessage + "\n").c_str());

}
