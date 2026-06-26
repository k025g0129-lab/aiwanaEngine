#pragma once
#include<cstdint>
#include<string>
#include <cmath> 
#include <cassert> 
#define _USE_MATH_DEFINES
#include <math.h>


struct Vector2 {
	float x, y;
};

struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

struct TransformSRT {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};	

struct Matrix4x4 {
	float m[4][4];
};

struct VertexData{
	Vector4 pos;
	Vector2 texcoord;
	Vector3 normal;
};

struct Material {
	Vector4 color;	
	int32_t enableLighting;
};

struct Sphere {
	Vector3 center;
	float radius;
};

struct TransformationMaterial {
	Matrix4x4 WVP;
	Matrix4x4 World;
};

struct DirectionalLight
{
	Vector4 color;
	Vector3 direction;
	float intensity;
};

//行列積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m1); 

//単位行列
Matrix4x4 MakeIdentity4x4(); 

Matrix4x4 MakeTranslateMatrix(const Vector3& translate); 

//拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

//座標変換
Vector3 TransformV3ToM4x4(const Vector3& vector, Matrix4x4 matrix); 

//各回転変換
Matrix4x4 MakeRotateXMatrix(float radian); 

Matrix4x4 MakeRotateYMatrix(float radian); 

Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);


void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
