#include "Sprite.h"
#include "DirectXCommon.h"
#include "WinApp.h"

#include <cassert>
#include <cstdint>
#include <algorithm>

void Sprite::Initialize(DirectXCommon* dxCommon, D3D12_GPU_DESCRIPTOR_HANDLE textureHandle) {

    dxCommon_ = dxCommon;
    textureHandle_ = textureHandle;


    //頂点
    vertexResource_ = dxCommon_->CreateBufferResource(sizeof(VertexData) * 4);

    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    VertexData* vertexData = nullptr;

    HRESULT hr = vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
    assert(SUCCEEDED(hr));

    // 左下
    vertexData[0].pos = { -320.0f, 180.0f, 0.0f, 1.0f };
    vertexData[0].texcoord = { 0.0f, 1.0f };

    // 左上
    vertexData[1].pos = { -320.0f,-180.0f, 0.0f, 1.0f };
    vertexData[1].texcoord = { 0.0f, 0.0f };

    // 右下
    vertexData[2].pos = { 320.0f, 180.0f, 0.0f, 1.0f };
    vertexData[2].texcoord = { 1.0f, 1.0f };

    // 右上
    vertexData[3].pos = { 320.0f, -180.0f, 0.0f, 1.0f };
    vertexData[3].texcoord = { 1.0f, 0.0f };

    for (int i = 0; i < 4; ++i) {
        vertexData[i].normal = { 0.0f, 0.0f, -1.0f };
    }


    //インデックス
    indexResource_ = dxCommon_->CreateBufferResource(sizeof(uint32_t) * 6);

    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    uint32_t* indexData = nullptr;

    hr = indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
    assert(SUCCEEDED(hr));

    indexData[0] = 0;
    indexData[1] = 1;
    indexData[2] = 2;
    indexData[3] = 1;
    indexData[4] = 3;
    indexData[5] = 2;


    //マテリアル
    materialResource_ = dxCommon->CreateBufferResource(sizeof(Material));
    hr = materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
    assert(SUCCEEDED(hr));

    materialData_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    materialData_->uvTransform = MakeIdentity4x4();
    materialData_->lightngType = 0;


    //座標変換行列
    transformationMatrixResource_ = dxCommon->CreateBufferResource(sizeof(TransformationMaterial));
    hr = transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
    assert(SUCCEEDED(hr));

    transformationMatrixData_->WVP = MakeIdentity4x4();
    transformationMatrixData_->World = MakeIdentity4x4();


    Update();
}

void Sprite::Update(){

    Matrix4x4 worldMatrix = MakeAffineMatrix(
        transform_.scale,
        transform_.rotate,
        transform_.translate
    );

    Matrix4x4 viewMatrix = MakeIdentity4x4();

    Matrix4x4 projectionMatrix = MakeOrthographicMatrix(
        0.0f,
        0.0f,
        float(WinApp::kClientWidth),
        float(WinApp::kClientHeight),
        0.0f,
        100.0f
    );

    Matrix4x4 wvpMatrix = Multiply(worldMatrix,Multiply(viewMatrix, projectionMatrix));



    transformationMatrixData_->World = worldMatrix;
    transformationMatrixData_->WVP = wvpMatrix;
}

void Sprite::Draw(){

    ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList->IASetIndexBuffer(&indexBufferView_);



    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootDescriptorTable(2, textureHandle_);

    commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);
}

void Sprite::SetSize(float w, float h){

    transform_.scale.x = w / 640.0f;
    transform_.scale.y = h / 360.0f;

}

void Sprite::SetAlpha(float a){
    if (materialData_ == nullptr) {
        return;
    }

    materialData_->color.w = std::clamp(a, 0.0f, 1.0f);
    

}
