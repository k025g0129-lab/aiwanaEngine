#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include<dxcapi.h>
#include"function.h"
#include "DebugLog.h"
#include "DebugLogMacro.h"
#include <array>
#include <string>
#include "externals/DirectXTex/DirectXTex.h"
#include "externals/DirectXTex/d3dx12.h"


#ifdef USE_IMGUI

#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
#endif // 





class DirectXCommon{
public:
	void Initialize(WinApp* winApp);

	void BeginDraw();
	void EndDraw();
	
	
	void CreateDevice();
	void CommandInitialize();
	void SwapChainCreate();
	void DepthBufferCreate();
	void DescriptorHeapsCreate();
	void RenderTargetViewInitialize();
	void DepthStencilViewInitialize();
	void FenceInitialize();
	void ViewportInitialize();
	void ScissorRectInitialize();
	void DXCCompilerCreate();
	void ImGuiInitialize();
	void ExecuteUploadCommands();

	Microsoft::WRL::ComPtr < ID3D12Resource> CreatDepthStencilTextureResource(Microsoft::WRL::ComPtr < ID3D12Device>& device, int32_t width, int32_t height);
	std::string ConvertString(const std::wstring& str);
	std::wstring ConvertString(const std::string& str);

	//デスクリプタヒープ生成
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap> CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);
	
	//デスクリプタハンドル取得
	static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(const Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);
	static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(const Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>& descriptorHeap, uint32_t descriptorSize, uint32_t index);

	//SRV特化
	D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUDescriptorHandle(uint32_t index);

	ID3D12Device* GetDevice() const { return device_.Get(); };
	ID3D12GraphicsCommandList* GetCommandList() const {return commandList_.Get();};

	Microsoft::WRL::ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath,const wchar_t* profile);
	Microsoft::WRL::ComPtr < ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	[[nodiscard]]
	Microsoft::WRL::ComPtr < ID3D12Resource> UploadTextureData(const Microsoft::WRL::ComPtr < ID3D12Resource>& texture, const DirectX::ScratchImage& mipImages);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
private:

	WinApp* winApp_ = nullptr;

	//デバイス
	//ファクトリ
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;

	//コマンド関連
	Microsoft::WRL::ComPtr < ID3D12CommandQueue> commandQueue_;
	Microsoft::WRL::ComPtr <ID3D12CommandAllocator>commandAllocator_;
	Microsoft::WRL::ComPtr < ID3D12GraphicsCommandList> commandList_;

	//スワップチェイン
	Microsoft::WRL::ComPtr < IDXGISwapChain4> swapChain_;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};

	//深度バッファ
	Microsoft::WRL::ComPtr < ID3D12Resource> depthStencilResource_;

	//デスクリプタヒープ
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>rtvDescriptorHeap_;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>srvDescriptorHeap_;
	Microsoft::WRL::ComPtr < ID3D12DescriptorHeap>dsvDescriptorHeap_;
	uint32_t desriptorSizeRTV_;
	uint32_t desriptorSizeSRV_; 
	uint32_t desriptorSizeDSV_;

	//レンダーターゲットビュー
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource>,2> swapChainResources;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};

	//深度ステンシルビュー
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};

	//フェンス
	Microsoft::WRL::ComPtr < ID3D12Fence>fence;
	uint64_t fenceValue;
	HANDLE fenceEvent;

	//ビューポート矩形
	D3D12_VIEWPORT viewport{};

	//シザリング矩形
	D3D12_RECT scissorRect{};

	//DXCコンパイラ
	IDxcUtils* dxcUtils;
	IDxcCompiler3* dxcCompiler;
	Microsoft::WRL::ComPtr < IDxcIncludeHandler> includeHandler_;

};

