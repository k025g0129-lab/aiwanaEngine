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

	if (input->PushKey(DIK_W)) {
		const float speed = 0.01f;

		Vector3 move = { speed,0,0 };

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_S)){
		const float speed = 0.01f;

		Vector3 move = {speed,0,0};

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate,move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_A)) {
		const float speed = 0.01f;

		Vector3 move = { 0,speed,0 };

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_D)) {
		const float speed = 0.01f;

		Vector3 move = { 0,0 ,speed};

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_Q)) {
		const float speed = 0.01f;

		Vector3 move = { 0,speed,0 };

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	if (input->PushKey(DIK_E)) {
		const float speed = 0.01f;

		Vector3 move = { 0,0 ,speed };

		cameraTransformSRT_.translate = AddV3(cameraTransformSRT_.translate, move);
		cameraTransformSRT_.rotate = AddV3(cameraTransformSRT_.rotate, move);

	}

	Matrix4x4 matRotDelta = MakeIdentity4x4();
	matRotDelta = Multiply(MakeRotateXMatrix(cameraTransformSRT_.rotate.x),matRotDelta);
	matRotDelta = Multiply(MakeRotateYMatrix(cameraTransformSRT_.rotate.y),matRotDelta);
	matRotDelta = Multiply(MakeRotateZMatrix(cameraTransformSRT_.rotate.z),matRotDelta);

	matRot_ = Multiply(matRotDelta, matRot_);

	Matrix4x4 matTrans = MakeTranslateMatrix(cameraTransformSRT_.translate);

		
	worldMatri_ = MakeAffineMatrix(transformSRT_.scale, transformSRT_.rotate, transformSRT_.translate);
	//cameraMatrix_ = MakeAffineMatrix(cameraTransformSRT_.scale, cameraTransformSRT_.rotate, cameraTransformSRT_.translate);
	cameraMatrix_ = Multiply(matRotDelta,matTrans);
	viewMatrix_ = Inverse(cameraMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(0.45f, float(kClientWidth_) / float(kClientHeight_), 0.1f, 100.0f);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
	worldViewProjectionMatrix_ = Multiply(worldMatri_, Multiply(viewMatrix_, projectionMatrix_));


}

Matrix4x4 DebugCamera::GetDebugWorldMatri(){

	return worldMatri_;
}

Matrix4x4 DebugCamera::GetDebugWorldViewProjectionMatrix_(){

	return worldViewProjectionMatrix_;
}

Matrix4x4 DebugCamera::GetDebugViewProjectionMatrix_(){

	return viewProjectionMatrix_;
}
