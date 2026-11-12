#include "Object3d.h"
#include "DirectXCommon.h"

#include <cassert>
#include <cstring>

void Object3d::Initialize(DirectXCommon* dxCommon, const ModelData& modelData, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle){

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


    //WVP
    wvpResource_ = dxCommon->CreateBufferResource(sizeof(TransformationMaterial));
    hr = wvpResource_->Map(0, nullptr, reinterpret_cast<void**>(&wvpData_));
    assert(SUCCEEDED(hr));

    wvpData_->World = MakeIdentity4x4();
    wvpData_->WVP = MakeIdentity4x4();

}

void Object3d::Update(const Matrix4x4 & viewMatrix, const Matrix4x4 & projectionMatrix){

    Matrix4x4 worldMatrix = MakeAffineMatrix(
        transform_.scale,
        transform_.rotate,
        transform_.translate
    );

    // WVP行列
    Matrix4x4 wvpMatrix = Multiply(
        worldMatrix,
        Multiply(viewMatrix, projectionMatrix)
    );

    wvpData_->World = worldMatrix;
    wvpData_->WVP = wvpMatrix;
}

void Object3d::Draw(){

    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);

    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, wvpResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootDescriptorTable(2, textureHandle_);

    commandList->DrawInstanced(vertexCount_, 1, 0, 0);
}
