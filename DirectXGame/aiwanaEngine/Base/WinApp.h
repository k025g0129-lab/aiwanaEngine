#pragma once
#include <Windows.h>
#include <objbase.h>
#include<cstdint>
#include <Windows.h>


#pragma comment(lib,"winmm.lib")

#ifdef USE_IMGUI

#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // 

class WinApp{

public:
	void Initialize();

	void Update();

	void Finalize();

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	HWND GetHwnd()const { return hwnd; };
	HINSTANCE GetHInstance()const { return wc.hInstance; };

	bool ProcessMessage();

private:
	HRESULT hr;
	WNDCLASS wc{};
	RECT wrc;
	HWND hwnd = nullptr;

};

