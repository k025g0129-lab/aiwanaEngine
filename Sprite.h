#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "function.h"

class DirectXCommon;

class Sprite{
public:
	void Initialize(DirectXCommon* dxCommon,D3D12_GPU_DESCRIPTOR_HANDLE textureHandle);
	void Update();
	void Draw();

	TransformSRT& GetTransform() {return transform_;}
	void SetSize(float w,float h);
	void SetAlpha(float a);


private:
	DirectXCommon* dxCommon_ = nullptr;

	//頂点
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	//インデックス
	Microsoft::WRL::ComPtr < ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

	//マテリアル
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;
	Material* materialData_ = nullptr;

	//座標変換行列
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResource_;
	TransformationMaterial* transformationMatrixData_ = nullptr;

	TransformSRT transform_{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{640.0f, 360.0f, 0.0f}
	
	};

	// テクスチャハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};

};

