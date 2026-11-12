#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "function.h"

class DirectXCommon;

class Object3d{
public:
	void Initialize(DirectXCommon* dxCommon, const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle);
	void Update(const Matrix4x4& viewMatrix,const Matrix4x4& projectionMatrix);
	void Draw();


	TransformSRT& GetTransform() { return transform_; }

private:

	DirectXCommon* dxCommon_ = nullptr;

	//頂点
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	UINT vertexCount_ = 0;

	//マテリアル
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;
	Material* materialData_ = nullptr;

	//座標変換
	Microsoft::WRL::ComPtr < ID3D12Resource> wvpResource_;
	TransformationMaterial* wvpData_ = nullptr;

	TransformSRT transform_{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	// テクスチャ
	D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};
};

