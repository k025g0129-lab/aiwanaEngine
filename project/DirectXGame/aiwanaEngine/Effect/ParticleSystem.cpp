#include "ParticleSystem.h"
#include "DirectXCommon.h"

#include <cassert>
#include <cstring>

void ParticleSystem::Initialize(DirectXCommon* dxCommon, const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle, uint32_t maxInstance){
    assert(maxInstance > 0);

    maxInstance_ = maxInstance;
    instanceCount_ = maxInstance_;

    transforms_.resize(maxInstance_);

    for (uint32_t i = 0; i < maxInstance_; ++i) {

        transforms_[i].scale = { 1.0f, 1.0f, 1.0f };
        transforms_[i].rotate = { 0.0f, 0.0f , 0.0f };

        transforms_[i].translate = {
            i * 0.1f,
            i * 0.1f,
            i * 0.1f
        };
    }

    dxCommon_ = dxCommon;
    textureHandle_ = textureHandle;

    //頂点数
    vertexCount_ = static_cast<UINT>(modelData.vertices.size());
    assert(vertexCount_ > 0);

    //バッファ
    vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * vertexCount_);

    //バッファービュー
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * vertexCount_;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);


    //頂点データ
    VertexData* vertexData = nullptr;

    HRESULT hr = vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    assert(SUCCEEDED(hr));
    std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * vertexCount_);


    //マテリアル
    materialResource_ = dxCommon->CreateBufferResource(sizeof(Material));
    hr = materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    assert(SUCCEEDED(hr));

    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->uvTransform = MakeIdentity4x4();
    materialData_->lightngType = 0;


    // Instancing用Resource
    instancingResource_ = dxCommon_->CreateBufferResource(sizeof(TransformationMaterial) * maxInstance_);

    // 書き込み先を取得
    hr = instancingResource_->Map( 0,nullptr,reinterpret_cast<void**>(&instancingData_));
    assert(SUCCEEDED(hr));

    // 全インスタンスの行列を初期化
    for (uint32_t i = 0; i < maxInstance_; ++i) {
        instancingData_[i].WVP = MakeIdentity4x4();
        instancingData_[i].World = MakeIdentity4x4();
    }


    // Instancing用SRVの設定
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

    srvDesc.Format = DXGI_FORMAT_UNKNOWN;
    srvDesc.Shader4ComponentMapping =
        D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.FirstElement = 0;
    srvDesc.Buffer.NumElements = maxInstance_;
    srvDesc.Buffer.StructureByteStride =sizeof(TransformationMaterial);
    srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

    // SRVのハンドル取得
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU =dxCommon_->GetSRVCPUDescriptorHandle(3);

    instancingHandleGPU_ =dxCommon_->GetSRVGPUDescriptorHandle(3);

    // SRVを生成
    dxCommon_->GetDevice()->CreateShaderResourceView(instancingResource_.Get(),&srvDesc,srvHandleCPU);
}

void ParticleSystem::Update(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix){

    Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

    Matrix4x4 emitterMatrix = MakeAffineMatrix(
        emitterTransform_.scale,
        emitterTransform_.rotate,
        emitterTransform_.translate
    );

    for (uint32_t i = 0; i < instanceCount_; i++){

        Matrix4x4 particleMatrix = MakeAffineMatrix(
            transforms_[i].scale,
            transforms_[i].rotate,
            transforms_[i].translate
        );

        Matrix4x4 worldMatrix = Multiply(particleMatrix, emitterMatrix);



        instancingData_[i].World = worldMatrix;
        instancingData_[i].WVP = Multiply(worldMatrix, viewProjectionMatrix);

    }




}

void ParticleSystem::Draw(){


    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

    assert(instanceCount_ <= maxInstance_);

    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootDescriptorTable(1, instancingHandleGPU_);
    commandList->SetGraphicsRootDescriptorTable(2, textureHandle_);

    commandList->DrawInstanced(vertexCount_, instanceCount_, 0, 0);

}
