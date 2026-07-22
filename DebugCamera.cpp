#include "DebugCamera.h"
#include <dinput.h>

DebugCamera::DebugCamera(){

}

void DebugCamera::Initialize(const int32_t& kClientWidth, const int32_t& kClientHeight, const TransformSRT transformSRT, const TransformSRT cameraTransformSRT){
	
	transformSRT_ = transformSRT;
	cameraTransformSRT_ = cameraTransformSRT;
	worldMatri_ = MakeAffineMatrix(transformSRT_.scale, transformSRT_.rotate, transformSRT_.translate);
	cameraMatrix_ = MakeAffineMatrix(cameraTransformSRT_.scale, cameraTransformSRT_.rotate, cameraTransformSRT_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	kClientWidth_ = kClientWidth;
	kClientHeight_ = kClientHeight;
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kClientWidth) / float(kClientHeight), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatri_, Multiply(viewMatrix_, projectionMatrix_));

}

void DebugCamera::Updata(){
	Input* input = Input::GetInstance();

	if (input->PushKey(DIK_W)){
		const float speed = 0.1f;

		Vector3 move = {0,0,speed};

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate,move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_D)) {
		const float speed = 0.1f;

		Vector3 move = { speed,0,0 };

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

		
	worldMatri_ = MakeAffineMatrix(transformSRT_.scale, transformSRT_.rotate, transformSRT_.translate);
	cameraMatrix_ = MakeAffineMatrix(cameraTransformSRT_.scale, cameraTransformSRT_.rotate, cameraTransformSRT_.translate);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	worldViewProjectionMatrix_ = Multiply(worldMatri_, Multiply(viewMatrix_, projectionMatrix_));


}

Matrix4x4 DebugCamera::GetDebugWorldMatri(){

	return worldMatri_;
}

Matrix4x4 DebugCamera::GetDebugWorldViewProjectionMatrix_(){

	return worldViewProjectionMatrix_;
}
