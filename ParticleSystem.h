#pragma once

#include <d3d12.h>
#include <wrl.h>
#include "function.h"
#include <vector>
#include <cstdint>

class DirectXCommon;


class ParticleSystem{
public:
	void Initialize(DirectXCommon* dxCommon, const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle, uint32_t maxInstance);
	void Update(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
	void Draw();



	TransformSRT& GetTransform() { return emitterTransform_; }

private:
	DirectXCommon* dxCommon_ = nullptr;

	uint32_t maxInstance_ = 0;
	uint32_t instanceCount_ = 0;


	TransformSRT emitterTransform_{
		{1.0f, 1.0f, 1.0f},
		{0.0f, -3.14f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	std::vector<TransformSRT> transforms_;

	Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource_;

	TransformationMaterial* instancingData_ = nullptr;

	D3D12_GPU_DESCRIPTOR_HANDLE instancingHandleGPU_{};


	//頂点
	Microsoft::WRL::ComPtr < ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	UINT vertexCount_ = 0;

	//マテリアル
	Microsoft::WRL::ComPtr < ID3D12Resource> materialResource_;
	Material* materialData_ = nullptr;

	// テクスチャ
	D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};
};

