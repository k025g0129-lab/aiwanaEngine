#include<Windows.h>
#include<cstdint>
#include<string>
#include<format>
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include "DebugLog.h"
#include "DebugLogMacro.h"
#include <dbghelp.h>
#include<strsafe.h>
#include<dxgidebug.h>
#include<dxcapi.h>
#include"function.h"
#include"externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
#include<wrl.h>
#include<xaudio2.h>
#include<fstream>
#include "Input.h"
#include "DebugCamera.h"
#include<vector>
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ParticleSystem.h"


#ifdef USE_IMGUI

#include"externals/imgui/imgui.h"
#include"externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // 


#pragma comment(lib,"dbghelp.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
	
//ウィンドウ作成
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd,msg,wparam,lparam)){
		return true;
	}


	switch (msg) {

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, msg, wparam, lparam);

}

//SEH...?らしい
static LONG WINAPI ExportDump(EXCEPTION_POINTERS* exception) {

	SYSTEMTIME time;
	GetLocalTime(&time);
	wchar_t filePath[MAX_PATH] = { 0 };
	CreateDirectory(L"./Dumps", nullptr);
	StringCchPrintfW(filePath, MAX_PATH, L"./Dumps/%04d-%02d%02d-%02d%02d.dmp", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	HANDLE dumpFileHandle = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	//processId(このexeのId)とクラッシュ(例外)の発生したthreadIdを取得
	DWORD processId = GetCurrentProcessId();
	DWORD threadId = GetCurrentThreadId();

	//設定情報を入力
	MINIDUMP_EXCEPTION_INFORMATION minidumpInformation{ 0 };
	minidumpInformation.ThreadId = threadId;
	minidumpInformation.ExceptionPointers = exception;
	minidumpInformation.ClientPointers = TRUE;

	//Dumpを出力。MiniDumpNormalは最低限の情報を出力するフラグ
	MiniDumpWriteDump(GetCurrentProcess(), processId, dumpFileHandle, MiniDumpNormal, &minidumpInformation, nullptr, nullptr);

	//他に関連づけられているSEH例外ハンドラ	があれば実行。通常はプロセスを終了する
	return EXCEPTION_EXECUTE_HANDLER;
}



struct D3DResourceLeakChecker{
	~D3DResourceLeakChecker()
	{
		//リソースリークチェック
		Microsoft::WRL::ComPtr < IDXGIDebug1> debug;

		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
			debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		}
	}
};


SoundData SoundLoadWave(const char* filename) {
	//HRESULT hr;

	//ファイルオープン
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	//データ読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	if (strncmp(riff.chunk.id,"RIFF",4) != 0){
		assert(0);
	}

	if (strncmp(riff.type, "WAVE", 4) != 0){
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));

	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	if (strncmp(data.id,"JUNK", 4) == 0){
		file.seekg(data.size, std::ios_base::cur);

		file.read((char*)&data, sizeof(data));

	}

	if (strncmp(data.id, "data", 4) != 0){
		assert(0);

	}

	char* pBuffer = new char[data.size];
	file.read(pBuffer,data.size);

	file.close();

	//音声を返す

	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	return soundData;
}

void SoundUnLoad(SoundData* soundData) {
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};

}

void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {

	HRESULT hr;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	hr = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(hr));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	hr = pSourceVoice->SubmitSourceBuffer(&buf);
	hr = pSourceVoice->Start();

	
}



int WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {



	//先頭推奨
	D3DResourceLeakChecker leakCheck;


	SetUnhandledExceptionFilter(ExportDump);



	//WinApp跡地
	WinApp* winApp = nullptr;

	winApp = new WinApp();
	winApp->Initialize();

	//DirectXCommonの初期化処理
	DirectXCommon* dxCommon = nullptr;

	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);


	//デバック
	DebugLog::Initialize();


	//DepthStencilState設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;


	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[3].Descriptor.ShaderRegister = 1;
	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);


	//Sampler設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr)) {
		LOG("{}", reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);

	}

	ID3D12RootSignature* rootSignature = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;


	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	//BlendState設定
	//加算合算にしているので後で元に戻す推奨
	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;	
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;	
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	//RasterizerState設定
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	//Shaderコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob> vertexShaderBlob = dxCommon->CompileShader(L"Object3D.VS.hlsl", L"vs_6_0");
	assert(vertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> pixelShaderBlob = dxCommon->CompileShader(L"Object3D.PS.hlsl", L"ps_6_0");
	assert(pixelShaderBlob != nullptr);

	//PSO生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature;
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = { vertexShaderBlob->GetBufferPointer(),vertexShaderBlob->GetBufferSize() };

	graphicsPipelineStateDesc.PS = { pixelShaderBlob->GetBufferPointer(),pixelShaderBlob->GetBufferSize() };

	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ID3D12PipelineState* graphicsPipelineState = nullptr;
	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));


	//particle
	D3D12_ROOT_PARAMETER rootParametersPar[4] = {};
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignaturePar{};
	descriptionRootSignaturePar.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	rootParametersPar[0] = rootParameters[0];
	rootParametersPar[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParametersPar[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParametersPar[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParametersPar[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);
	rootParametersPar[2] = rootParameters[2];
	rootParametersPar[3] = rootParameters[3];


	descriptionRootSignaturePar.pParameters = rootParametersPar;
	descriptionRootSignaturePar.NumParameters = _countof(rootParametersPar);

	descriptionRootSignaturePar.pStaticSamplers = staticSamplers;
	descriptionRootSignaturePar.NumStaticSamplers = _countof(staticSamplers);

	ID3DBlob* signatureBlobOb3 = nullptr;
	ID3DBlob* errorBlobOb3 = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlobOb3, &errorBlobOb3);

	if (FAILED(hr)) {
		LOG("{}", reinterpret_cast<char*>(errorBlobOb3->GetBufferPointer()));
		assert(false);

	}

	ID3D12RootSignature* rootSignatureOb3 = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlobOb3->GetBufferPointer(), signatureBlobOb3->GetBufferSize(), IID_PPV_ARGS(&rootSignatureOb3));
	assert(SUCCEEDED(hr));


	ID3DBlob* signatureBlobPar = nullptr;
	ID3DBlob* errorBlobPar = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignaturePar, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlobPar, &errorBlobPar);

	if (FAILED(hr)) {
		LOG("{}", reinterpret_cast<char*>(errorBlobPar->GetBufferPointer()));
		assert(false);

	}

	ID3D12RootSignature* rootSignaturePar = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlobPar->GetBufferPointer(), signatureBlobPar->GetBufferSize(), IID_PPV_ARGS(&rootSignaturePar));
	assert(SUCCEEDED(hr));



	// Particle Shaderコンパイル
	Microsoft::WRL::ComPtr<IDxcBlob> particleVertexShaderBlob =
		dxCommon->CompileShader(
			L"Particle.VS.hlsl",
			L"vs_6_0"
		);

	assert(particleVertexShaderBlob != nullptr);

	Microsoft::WRL::ComPtr<IDxcBlob> particlePixelShaderBlob =
		dxCommon->CompileShader(
			L"Particle.PS.hlsl",
			L"ps_6_0"
		);

	assert(particleVertexShaderBlob != nullptr);
	assert(particlePixelShaderBlob != nullptr);


	// Object3D用PSOをコピーしてParticle用に変更
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDescPar = graphicsPipelineStateDesc;

	graphicsPipelineStateDescPar.pRootSignature = rootSignaturePar;

	graphicsPipelineStateDescPar.VS = {
		particleVertexShaderBlob->GetBufferPointer(),
		particleVertexShaderBlob->GetBufferSize()
	};

	graphicsPipelineStateDescPar.PS = {
		particlePixelShaderBlob->GetBufferPointer(),
		particlePixelShaderBlob->GetBufferSize()
	};

	// 板ポリを両面描画
	graphicsPipelineStateDescPar.RasterizerState.CullMode =
		D3D12_CULL_MODE_NONE;

	// 深度バッファには書き込まない
	graphicsPipelineStateDescPar.DepthStencilState.DepthWriteMask =
		D3D12_DEPTH_WRITE_MASK_ZERO;


	ID3D12PipelineState* graphicsPipelineStatePar =
		nullptr;

	hr = dxCommon->GetDevice()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDescPar,
		IID_PPV_ARGS(&graphicsPipelineStatePar)
	);

	assert(SUCCEEDED(hr));

	//OBJ読み込み
	ModelData modelData = LoadObjFile("resources/fence", "fence.obj");
	ModelData modelData2 = LoadObjFile("resources", "plane.obj");

	//マテリアル用に変更
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * modelData.vertices.size());
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource = dxCommon->CreateBufferResource( sizeof(Material));
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource = dxCommon->CreateBufferResource( sizeof(TransformationMaterial));


	//頂点バッファビュー作成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	Material* materialData = nullptr;
	TransformationMaterial* wvpData = nullptr;
		
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	//封印
	////1枚目
	////左下
	//vertexData[0].pos = { -0.5f,-0.5f,0.0f,1.0f };
	//vertexData[0].texcoord = { 0.0f,1.0f };

	////上
	//vertexData[1].pos = { 0.0f,0.5f,0.0f,1.0f };
	//vertexData[1].texcoord = { 0.5f,0.0f };

	////右下
	//vertexData[2].pos = { 0.5f,-0.5f,0.0f,1.0f };
	//vertexData[2].texcoord = { 1.0f,1.0f };

	////2枚目
	////左
	//vertexData[3].pos = { -0.5f,-0.5f,0.5f,1.0f };
	//vertexData[3].texcoord = { 0.0f,1.0f };

	////上
	//vertexData[4].pos = { 0.0f,0.0f,0.0f,1.0f };
	//vertexData[4].texcoord = { 0.5f,0.0f };

	////右下
	//vertexData[5].pos = { 0.5f,-0.5f,-0.5f,1.0f };
	//vertexData[5].texcoord = { 1.0f,1.0f };

	materialData->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData->lightngType = 2;
	materialData->uvTransform = MakeIdentity4x4();

	wvpData->World = MakeIdentity4x4();
	wvpData->WVP = MakeIdentity4x4();

	//スプライト用頂点リソース

	Microsoft::WRL::ComPtr < ID3D12Resource> directionalLightResourceSphere = dxCommon->CreateBufferResource( sizeof(DirectionalLight));

	directionalLightResourceSphere->SetName(L"Sphere:directionalLightResource");

	DirectionalLight* directionalLightDataSphere = nullptr;

	directionalLightResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightDataSphere));

	directionalLightDataSphere->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightDataSphere->direction = { 0.0f,-1.0f,0.0f };
	directionalLightDataSphere->intensity = 1.0f;

	//Transform変数作成(3D,2D)
	TransformSRT transformSRT = { {1.0f,1.0f,1.0f},{0.0f,3.0f,0.0f},{0.0f,0.0f,0.0f} };


	//3次元的用変数	
	TransformSRT cameraTransformSRT = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-10.0f} };
	Matrix4x4 worldMatri = MakeAffineMatrix(transformSRT.scale, transformSRT.rotate, transformSRT.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransformSRT.scale, cameraTransformSRT.rotate, cameraTransformSRT.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(WinApp::kClientWidth) / float(WinApp::kClientHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatri, Multiply(viewMatrix, projectionMatrix));
	wvpData->WVP = worldViewProjectionMatrix;
	wvpData->World = worldMatri;

	//何のテクスチャを貼るか
	DirectX::ScratchImage mipImages = dxCommon->LoadTexture("resources/uvChecker.png");
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource = dxCommon->CreateTextureResource( metadata);
	Microsoft::WRL::ComPtr < ID3D12Resource> a1 = dxCommon->UploadTextureData(textureResource, mipImages);

	DirectX::ScratchImage mipImages2 = dxCommon->LoadTexture(modelData.material.textureFilrPath);
	const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	Microsoft::WRL::ComPtr < ID3D12Resource> textureResource2 = dxCommon->CreateTextureResource( metadata2);
	Microsoft::WRL::ComPtr < ID3D12Resource> a2 = dxCommon->UploadTextureData(textureResource2, mipImages2);

	//
	////削り取られた跡地
	//
	dxCommon->ExecuteUploadCommands();


	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//2個目
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);


	//描画してないので一時的にコメント化
	//const uint32_t desriptorSizeSRV = dxCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	
	
	//const uint32_t desriptorSizeRTV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//const uint32_t desriptorSizeDSV = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU = dxCommon->GetSRVCPUDescriptorHandle(1);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU = dxCommon->GetSRVGPUDescriptorHandle(1);

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2 = dxCommon->GetSRVCPUDescriptorHandle(2);
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2 = dxCommon->GetSRVGPUDescriptorHandle(2);

	dxCommon->GetDevice()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	dxCommon->GetDevice()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);


	Sprite sprite;
	sprite.Initialize(dxCommon, textureSrvHandleGPU2);

	Object3d object3d;
	object3d.Initialize(dxCommon,modelData,textureSrvHandleGPU2);
	object3d.GetTransform().rotate.y = 3.0f;

	ParticleSystem particle;
	particle.Initialize(dxCommon, modelData2, textureSrvHandleGPU,10);

	// サイズ変更
	sprite.SetSize(320.0f, 180.0f);

	// 透明度
	sprite.SetAlpha(0.7f);
	//球(コメントする可能性あり)


	bool useSphere = false;
	const uint32_t kSubdivision = 16;


	TransformSRT transformSphere = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	Microsoft::WRL::ComPtr<ID3D12Resource> sphereWvpResource = dxCommon->CreateBufferResource(sizeof(TransformationMaterial));

	TransformationMaterial* sphereWvpData = nullptr;

	sphereWvpResource->Map(0,nullptr,reinterpret_cast<void**>(&sphereWvpData));


	Matrix4x4 sphereWorld =MakeAffineMatrix(transformSphere.scale,transformSphere.rotate,transformSphere.translate);

	Matrix4x4 sphereWVP =Multiply(sphereWorld,Multiply(viewMatrix, projectionMatrix));

	sphereWvpData->World = sphereWorld;
	sphereWvpData->WVP = sphereWVP;

	Microsoft::WRL::ComPtr < ID3D12Resource> sphereVertexResource = dxCommon->CreateBufferResource(sizeof(VertexData) * kSubdivision * kSubdivision * 6);
	D3D12_VERTEX_BUFFER_VIEW sphereVertexBufferView{};
	sphereVertexBufferView.BufferLocation = sphereVertexResource->GetGPUVirtualAddress();
	sphereVertexBufferView.SizeInBytes =sizeof(VertexData) * kSubdivision * kSubdivision * 6;
	sphereVertexBufferView.StrideInBytes = sizeof(VertexData);
	VertexData* sphereVertexData = nullptr;

	sphereVertexResource->Map(0, nullptr, reinterpret_cast<void**>(&sphereVertexData));

	Sphere sphere;
	sphere.center = { 0.0f,0.0f,0.0f };
	sphere.radius = 1.0f;

	const float kLonEvery = 2.0f * static_cast<float>(M_PI) / static_cast<float>(kSubdivision);
	const float kLatEvery = static_cast<float>(M_PI) / static_cast<float>(kSubdivision);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {

		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {

			uint32_t start = (latIndex * kSubdivision + lonIndex) * 6;
			float lon = lonIndex * kLonEvery;

			float u0 = float(lonIndex) / float(kSubdivision);
			float u1 = float(lonIndex + 1) / float(kSubdivision);

			float v0 = 1.0f - float(latIndex) / float(kSubdivision);
			float v1 = 1.0f - float(latIndex + 1) / float(kSubdivision);

			//1a
			sphereVertexData[start].pos.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon);
			sphereVertexData[start].pos.y = sphere.center.y + sphere.radius * sinf(lat);
			sphereVertexData[start].pos.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon);
			sphereVertexData[start].pos.w = 1.0f;
			sphereVertexData[start].texcoord = { u0,v0 };
			sphereVertexData[start].normal.x = sphereVertexData[start].pos.x;
			sphereVertexData[start].normal.y = sphereVertexData[start].pos.y;
			sphereVertexData[start].normal.z = sphereVertexData[start].pos.z;

			//1b
			sphereVertexData[start + 1].pos.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon);
			sphereVertexData[start + 1].pos.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			sphereVertexData[start + 1].pos.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon);
			sphereVertexData[start + 1].pos.w = 1.0f;
			sphereVertexData[start + 1].texcoord = { u0,v1 };
			sphereVertexData[start + 1].normal.x = sphereVertexData[start + 1].pos.x;
			sphereVertexData[start + 1].normal.y = sphereVertexData[start + 1].pos.y;
			sphereVertexData[start + 1].normal.z = sphereVertexData[start + 1].pos.z;

			//1c
			sphereVertexData[start + 2].pos.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery);
			sphereVertexData[start + 2].pos.y = sphere.center.y + sphere.radius * sinf(lat);
			sphereVertexData[start + 2].pos.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery);
			sphereVertexData[start + 2].pos.w = 1.0f;
			sphereVertexData[start + 2].texcoord = { u1,v0 };
			sphereVertexData[start + 2].normal.x = sphereVertexData[start + 2].pos.x;
			sphereVertexData[start + 2].normal.y = sphereVertexData[start + 2].pos.y;
			sphereVertexData[start + 2].normal.z = sphereVertexData[start + 2].pos.z;

			//2b
			sphereVertexData[start + 3].pos.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon);
			sphereVertexData[start + 3].pos.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			sphereVertexData[start + 3].pos.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon);
			sphereVertexData[start + 3].pos.w = 1.0f;
			sphereVertexData[start + 3].texcoord = { u0,v1 };
			sphereVertexData[start + 3].normal.x = sphereVertexData[start + 3].pos.x;
			sphereVertexData[start + 3].normal.y = sphereVertexData[start + 3].pos.y;
			sphereVertexData[start + 3].normal.z = sphereVertexData[start + 3].pos.z;

			//2d
			sphereVertexData[start + 4].pos.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon + kLonEvery);
			sphereVertexData[start + 4].pos.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			sphereVertexData[start + 4].pos.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon + kLonEvery);
			sphereVertexData[start + 4].pos.w = 1.0f;
			sphereVertexData[start + 4].texcoord = { u1,v1 };
			sphereVertexData[start + 4].normal.x = sphereVertexData[start + 4].pos.x;
			sphereVertexData[start + 4].normal.y = sphereVertexData[start + 4].pos.y;
			sphereVertexData[start + 4].normal.z = sphereVertexData[start + 4].pos.z;

			//2c
			sphereVertexData[start + 5].pos.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery);
			sphereVertexData[start + 5].pos.y = sphere.center.y + sphere.radius * sinf(lat);
			sphereVertexData[start + 5].pos.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery);
			sphereVertexData[start + 5].pos.w = 1.0f;
			sphereVertexData[start + 5].texcoord = { u1,v0 };
			sphereVertexData[start + 5].normal.x = sphereVertexData[start + 5].pos.x;
			sphereVertexData[start + 5].normal.y = sphereVertexData[start + 5].pos.y;
			sphereVertexData[start + 5].normal.z = sphereVertexData[start + 5].pos.z;

		}
	}

	//サウンド
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	
	hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	assert(SUCCEEDED(hr));

	hr = xAudio2->CreateMasteringVoice(&masterVoice);

	assert(SUCCEEDED(hr));

	SoundData soundData1 = SoundLoadWave("./resources/sound/Alarm01.wav");

	//音鳴らす
	SoundPlayWave(xAudio2.Get(), soundData1);

	//キーボード
	Input::GetInstance()->Initialize(winApp);
	Input* input = Input::GetInstance();

	//デバックカメラ
	#ifdef _DEBUG
	DebugCamera* debugCamera = new DebugCamera();
	debugCamera->Initialize(WinApp::kClientWidth, WinApp::kClientHeight, transformSRT, cameraTransformSRT);
	bool isDebugOn = false;

	LightingType lightingType = HalfLambert;
	#endif 



	//メインループ

	while (true) {

		if (winApp->ProcessMessage()) {

			break;

		}

#ifdef USE_IMGUI

		//フレームの先頭?
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		//ImGui::Checkbox("useMonsterBall", &useMonsterBall);
		//ImGui::Checkbox("debugCamera", &isDebugOn);
		if (ImGui::TreeNode("Camera")) {

			ImGui::DragFloat3(
				"CameraTranslate",
				&cameraTransformSRT.translate.x,
				0.1f
			);

			ImGui::DragFloat3(
				"CameraRotate",
				&cameraTransformSRT.rotate.x,
				0.01f
			);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("2D")) {
			ImGui::DragFloat3("SpriteTransform", &sprite.GetTransform().translate.x, 0.1f);
			ImGui::DragFloat3("SpriteRotate", &sprite.GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("SpriteScale", &sprite.GetTransform().scale.x, 0.01f, -10.0f, 10.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("OBJ")) {
			ImGui::DragFloat3("OBJTranslate", &object3d.GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("OBJRotate", &object3d.GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("OBJScale", &object3d.GetTransform().scale.x, 0.01f, -10.0f, 10.0f);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Par")) {
			ImGui::DragFloat3("OBJTranslate", &particle.GetTransform().translate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("OBJRotate", &particle.GetTransform().rotate.x, 0.01f, -10.0f, 10.0f);
			ImGui::DragFloat3("OBJScale", &particle.GetTransform().scale.x, 0.01f, -10.0f, 10.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere")) {
			if (!useSphere) {
				if (ImGui::Button("useSphere")) {
					useSphere = true;
				}
			}
			else {
				ImGui::DragFloat3("SphereTranslate", &transformSphere.translate.x, 0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("SphereRotate", &transformSphere.rotate.x, 0.01f, -10.0f, 10.0f);
				ImGui::DragFloat3("SphereScale", &transformSphere.scale.x, 0.01f, -10.0f, 10.0f);
			}


			ImGui::TreePop();
		}



		if (ImGui::TreeNode("Light")) {
			ImGui::ColorEdit3("color", &directionalLightDataSphere->color.x);
			ImGui::DragFloat3("direction", &directionalLightDataSphere->direction.x, 0.01f, -1.0f, 1.0f);
			ImGui::DragFloat("Intensity", &directionalLightDataSphere->intensity, 0.01f, 0.0f, 10.0f);

			const char* items[] =
			{
				"None",
				"Lambert",
				"Half Lambert"
			};


			ImGui::Combo("Lighting Type",
				(int*)&lightingType,
				items,
				IM_ARRAYSIZE(items));
			//ImGui::Combo("Lighting Type",&lightingType,3);
			ImGui::TreePop();

		}

		materialData->lightngType = lightingType;


#endif // USE_IMGUI

		//transformSRT.rotate.y += 0.01f;

		//カメラ
		cameraMatrix = MakeAffineMatrix(
			cameraTransformSRT.scale,
			cameraTransformSRT.rotate,
			cameraTransformSRT.translate
		);

		viewMatrix = Inverse(cameraMatrix);
	
		object3d.Update(viewMatrix, projectionMatrix);
		particle.Update(viewMatrix, projectionMatrix);
	 
		//3D
		worldMatri = MakeAffineMatrix(transformSRT.scale, transformSRT.rotate, transformSRT.translate);
		worldViewProjectionMatrix = Multiply(worldMatri, Multiply(viewMatrix, projectionMatrix));

		//球
		sphereWvpResource->Map(0, nullptr, reinterpret_cast<void**>(&sphereWvpData));
		sphereWorld = MakeAffineMatrix(transformSphere.scale, transformSphere.rotate, transformSphere.translate);
		sphereWVP = Multiply(sphereWorld, Multiply(viewMatrix, projectionMatrix));

		//directionalLightDataSphere->direction = Normalize(directionalLightDataSphere->direction);

		wvpData->World = worldMatri;
		wvpData->WVP = worldViewProjectionMatrix;

		sphereWvpData->World = sphereWorld;
		sphereWvpData->WVP = sphereWVP;


		sprite.GetTransform().rotate.z += 0.02f;
		sprite.Update();




#ifdef _DEBUG //デバックカメラ

		if (isDebugOn) {


			debugCamera->Updata();

			wvpData->World = worldMatri;
			Matrix4x4 debugWorldViewProjectionMatrix = Multiply(worldMatri, debugCamera->GetDebugViewProjectionMatrix_());
			wvpData->WVP = debugWorldViewProjectionMatrix;


		}

#endif 
		
		dxCommon->BeginDraw();

		Input::GetInstance()->Update();


#ifdef USE_IMGUI

		//ImGui内部コマンド生成
		ImGui::Render();



#endif // DEBUG



		//更新処理
		if (input->PushKey(DIK_0)) {
			OutputDebugStringA("Hit 0\n");
		}




		//描画	

		ID3D12GraphicsCommandList* commandList = dxCommon->GetCommandList();

		//Object3D用
		commandList->SetGraphicsRootSignature(rootSignature);
		commandList->SetPipelineState(graphicsPipelineState);
		
		commandList->SetGraphicsRootConstantBufferView(3, directionalLightResourceSphere->GetGPUVirtualAddress());


		//object3d.Draw();
		//sprite.Draw();


		//Particle用
		commandList->SetGraphicsRootSignature(rootSignaturePar);
		commandList->SetPipelineState(graphicsPipelineStatePar);

		commandList->SetGraphicsRootConstantBufferView(3,directionalLightResourceSphere->GetGPUVirtualAddress());
		particle.Draw();

		//commandList->DrawInstanced(UINT(modelData.vertices.size()), 1, 0, 0);

		////球

		//if (useSphere) {
		//	commandList->IASetVertexBuffers(0, 1, &sphereVertexBufferView);
		//	commandList->SetGraphicsRootConstantBufferView(1, sphereWvpResource->GetGPUVirtualAddress());
		//	//commandList->SetGraphicsRootConstantBufferView(3, directionalLightResourceSphere->GetGPUVirtualAddress());

		//	commandList->DrawInstanced(kSubdivision * kSubdivision * 6, 1, 0, 0);
		//}


#ifdef USE_IMGUI
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), dxCommon->GetCommandList());


#endif // USE_IMGUI			


		dxCommon->EndDraw();

	}

	

	//解放処理
	#ifdef _DEBUG
	delete debugCamera;
	#endif 

	winApp->Finalize();
	delete winApp;

	delete dxCommon;

	xAudio2.Reset();
	SoundUnLoad(&soundData1);

	#ifdef USE_IMGUI

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	#endif // USE_IMGUI

	DebugLog::Shutdown();

	CoUninitialize();

	return 0;
}


