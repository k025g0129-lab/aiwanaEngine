#pragma once
#include "function.h"
#include "Input.h"


class DebugCamera{
public:

	DebugCamera();
	void Initialize(const int32_t& kClientWidth, const int32_t& kClientHeight,const TransformSRT transformSRT, const TransformSRT cameraTransformSRT);

	void Updata();

	Matrix4x4 GetDebugWorldMatri();
	Matrix4x4 GetDebugWorldViewProjectionMatrix_();

private:

	Vector3 rotation_ = {0,0,0};
	Vector3 translation_ = {0,0,-50};
	
	//ビュー行列
	TransformSRT transformSRT_;
	TransformSRT cameraTransformSRT_;
	int32_t kClientWidth_;
	int32_t kClientHeight_;
	Matrix4x4 worldMatri_ = MakeAffineMatrix(transformSRT_.scale, transformSRT_.rotate, transformSRT_.translate);
	Matrix4x4 cameraMatrix_ = MakeAffineMatrix(cameraTransformSRT_.scale, cameraTransformSRT_.rotate, cameraTransformSRT_.translate);
	Matrix4x4 viewMatrix_ = Inverse(cameraMatrix_);
	Matrix4x4 projectionMatrix_; 
	Matrix4x4 worldViewProjectionMatrix_;
		
};

