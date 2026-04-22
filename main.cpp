#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<filesystem>
#include<fstream>
#include<chrono>


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	switch (msg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, msg, wparam, lparam);

}


void Log(std::ostream& os, const std::string& message) {

	os << message << std::endl;
	OutputDebugStringA(message.c_str());

}


//デバック?のテキスト用
std::wstring ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

int WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR,_In_ int ){

	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"CG2WindowClass";
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	RegisterClass(&wc);

	const int32_t kClientWidth = 1280;
	const int32_t kClientHeight = 720;

	RECT wrc = { 0,0,kClientWidth ,kClientHeight };
	AdjustWindowRect(&wrc,WS_OVERLAPPEDWINDOW,false);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"CG2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr,
		
	);
	
	ShowWindow(hwnd, SW_SHOW);


	//OutputDebugStringA("Hello,DirectX!\n");
	
	MSG msg{};
	while (msg.message != WM_QUIT){
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {


		}

	}


	//デバック
	int a = 10;

	//Log(,ConvertString(std::format(L"enemyHp:{},texturePath:{}\n",a,a)));

	//デバックログ
	std::filesystem::create_directory("logs");

	//後に関数化予定
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::chrono::time_point < std::chrono::system_clock, std::chrono::seconds > nowSeconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
	std::chrono::zoned_time localTime{std::chrono::current_zone(),nowSeconds };
	std::string dateString = std::format("{:%Y%m%d_%H%M%S}",localTime);
	std::string logFilePath = std::string("logs/") + dateString + ".log";
	std::ofstream logStream(logFilePath);
	Log(logStream, ConvertString(std::format(L"enemyHp:{},texturePath:{}\n", a, a)));

	return 0;
}


