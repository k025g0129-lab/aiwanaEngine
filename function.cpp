#include "function.h"

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2){
	Matrix4x4 m3;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m3.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];

		}
	}

	return m3;
}

Matrix4x4 Inverse(const Matrix4x4& m1){
	Matrix4x4 m2;
	float det = m1.m[0][0] * (m1.m[1][1] * m1.m[2][2] * m1.m[3][3] + m1.m[1][2] * m1.m[2][3] * m1.m[3][1] + m1.m[1][3] * m1.m[2][1] * m1.m[3][2] - m1.m[1][3] * m1.m[2][2] * m1.m[3][1] - m1.m[1][2] * m1.m[2][1] * m1.m[3][3] - m1.m[1][1] * m1.m[2][3] * m1.m[3][2])
		- m1.m[0][1] * (m1.m[1][0] * m1.m[2][2] * m1.m[3][3] + m1.m[1][2] * m1.m[2][3] * m1.m[3][0] + m1.m[1][3] * m1.m[2][0] * m1.m[3][2] - m1.m[1][3] * m1.m[2][2] * m1.m[3][0] - m1.m[1][0] * m1.m[2][3] * m1.m[3][2] - m1.m[1][2] * m1.m[2][0] * m1.m[3][3])
		+ m1.m[0][2] * (m1.m[1][0] * m1.m[2][1] * m1.m[3][3] + m1.m[1][1] * m1.m[2][3] * m1.m[3][0] + m1.m[1][3] * m1.m[2][0] * m1.m[3][1] - m1.m[1][3] * m1.m[2][1] * m1.m[3][0] - m1.m[1][0] * m1.m[2][3] * m1.m[3][1] - m1.m[1][1] * m1.m[2][0] * m1.m[3][3])
		- m1.m[0][3] * (m1.m[1][0] * m1.m[2][1] * m1.m[3][2] + m1.m[1][1] * m1.m[2][2] * m1.m[3][0] + m1.m[1][2] * m1.m[2][0] * m1.m[3][1] - m1.m[1][2] * m1.m[2][1] * m1.m[3][0] - m1.m[1][0] * m1.m[2][2] * m1.m[3][1] - m1.m[1][1] * m1.m[2][0] * m1.m[3][2]);

	assert(det != 0.0f);
	float invDet = 1.0f / det;

	m2.m[0][0] = invDet * (m1.m[1][1] * m1.m[2][2] * m1.m[3][3] + m1.m[1][2] * m1.m[2][3] * m1.m[3][1] + m1.m[1][3] * m1.m[2][1] * m1.m[3][2] - m1.m[1][3] * m1.m[2][2] * m1.m[3][1] - m1.m[1][1] * m1.m[2][3] * m1.m[3][2] - m1.m[1][2] * m1.m[2][1] * m1.m[3][3]);
	m2.m[0][1] = invDet * -(m1.m[0][1] * m1.m[2][2] * m1.m[3][3] + m1.m[0][2] * m1.m[2][3] * m1.m[3][1] + m1.m[0][3] * m1.m[2][1] * m1.m[3][2] - m1.m[0][3] * m1.m[2][2] * m1.m[3][1] - m1.m[0][1] * m1.m[2][3] * m1.m[3][2] - m1.m[0][2] * m1.m[2][1] * m1.m[3][3]);
	m2.m[0][2] = invDet * (m1.m[0][1] * m1.m[1][2] * m1.m[3][3] + m1.m[0][2] * m1.m[1][3] * m1.m[3][1] + m1.m[0][3] * m1.m[1][1] * m1.m[3][2] - m1.m[0][3] * m1.m[1][2] * m1.m[3][1] - m1.m[0][1] * m1.m[1][3] * m1.m[3][2] - m1.m[0][2] * m1.m[1][1] * m1.m[3][3]);
	m2.m[0][3] = invDet * -(m1.m[0][1] * m1.m[1][2] * m1.m[2][3] + m1.m[0][2] * m1.m[1][3] * m1.m[2][1] + m1.m[0][3] * m1.m[1][1] * m1.m[2][2] - m1.m[0][3] * m1.m[1][2] * m1.m[2][1] - m1.m[0][1] * m1.m[1][3] * m1.m[2][2] - m1.m[0][2] * m1.m[1][1] * m1.m[2][3]);

	m2.m[1][0] = invDet * -(m1.m[1][0] * m1.m[2][2] * m1.m[3][3] + m1.m[1][2] * m1.m[2][3] * m1.m[3][0] + m1.m[1][3] * m1.m[2][0] * m1.m[3][2] - m1.m[1][3] * m1.m[2][2] * m1.m[3][0] - m1.m[1][0] * m1.m[2][3] * m1.m[3][2] - m1.m[1][2] * m1.m[2][0] * m1.m[3][3]);
	m2.m[1][1] = invDet * (m1.m[0][0] * m1.m[2][2] * m1.m[3][3] + m1.m[0][2] * m1.m[2][3] * m1.m[3][0] + m1.m[0][3] * m1.m[2][0] * m1.m[3][2] - m1.m[0][3] * m1.m[2][2] * m1.m[3][0] - m1.m[0][0] * m1.m[2][3] * m1.m[3][2] - m1.m[0][2] * m1.m[2][0] * m1.m[3][3]);
	m2.m[1][2] = invDet * -(m1.m[0][0] * m1.m[1][2] * m1.m[3][3] + m1.m[0][2] * m1.m[1][3] * m1.m[3][0] + m1.m[0][3] * m1.m[1][0] * m1.m[3][2] - m1.m[0][3] * m1.m[1][2] * m1.m[3][0] - m1.m[0][0] * m1.m[1][3] * m1.m[3][2] - m1.m[0][2] * m1.m[1][0] * m1.m[3][3]);
	m2.m[1][3] = invDet * (m1.m[0][0] * m1.m[1][2] * m1.m[2][3] + m1.m[0][2] * m1.m[1][3] * m1.m[2][0] + m1.m[0][3] * m1.m[1][0] * m1.m[2][2] - m1.m[0][3] * m1.m[1][2] * m1.m[2][0] - m1.m[0][0] * m1.m[1][3] * m1.m[2][2] - m1.m[0][2] * m1.m[1][0] * m1.m[2][3]);

	m2.m[2][0] = invDet * (m1.m[1][0] * m1.m[2][1] * m1.m[3][3] + m1.m[1][1] * m1.m[2][3] * m1.m[3][0] + m1.m[1][3] * m1.m[2][0] * m1.m[3][1] - m1.m[1][3] * m1.m[2][1] * m1.m[3][0] - m1.m[1][0] * m1.m[2][3] * m1.m[3][1] - m1.m[1][1] * m1.m[2][0] * m1.m[3][3]);
	m2.m[2][1] = invDet * -(m1.m[0][0] * m1.m[2][1] * m1.m[3][3] + m1.m[0][1] * m1.m[2][3] * m1.m[3][0] + m1.m[0][3] * m1.m[2][0] * m1.m[3][1] - m1.m[0][3] * m1.m[2][1] * m1.m[3][0] - m1.m[0][0] * m1.m[2][3] * m1.m[3][1] - m1.m[0][1] * m1.m[2][0] * m1.m[3][3]);
	m2.m[2][2] = invDet * (m1.m[0][0] * m1.m[1][1] * m1.m[3][3] + m1.m[0][1] * m1.m[1][3] * m1.m[3][0] + m1.m[0][3] * m1.m[1][0] * m1.m[3][1] - m1.m[0][3] * m1.m[1][1] * m1.m[3][0] - m1.m[0][0] * m1.m[1][3] * m1.m[3][1] - m1.m[0][1] * m1.m[1][0] * m1.m[3][3]);
	m2.m[2][3] = invDet * -(m1.m[0][0] * m1.m[1][1] * m1.m[2][3] + m1.m[0][1] * m1.m[1][3] * m1.m[2][0] + m1.m[0][3] * m1.m[1][0] * m1.m[2][1] - m1.m[0][3] * m1.m[1][1] * m1.m[2][0] - m1.m[0][0] * m1.m[1][3] * m1.m[2][1] - m1.m[0][1] * m1.m[1][0] * m1.m[2][3]);

	m2.m[3][0] = invDet * -(m1.m[1][0] * m1.m[2][1] * m1.m[3][2] + m1.m[1][1] * m1.m[2][2] * m1.m[3][0] + m1.m[1][2] * m1.m[2][0] * m1.m[3][1] - m1.m[1][2] * m1.m[2][1] * m1.m[3][0] - m1.m[1][0] * m1.m[2][2] * m1.m[3][1] - m1.m[1][1] * m1.m[2][0] * m1.m[3][2]);
	m2.m[3][1] = invDet * (m1.m[0][0] * m1.m[2][1] * m1.m[3][2] + m1.m[0][1] * m1.m[2][2] * m1.m[3][0] + m1.m[0][2] * m1.m[2][0] * m1.m[3][1] - m1.m[0][2] * m1.m[2][1] * m1.m[3][0] - m1.m[0][0] * m1.m[2][2] * m1.m[3][1] - m1.m[0][1] * m1.m[2][0] * m1.m[3][2]);
	m2.m[3][2] = invDet * -(m1.m[0][0] * m1.m[1][1] * m1.m[3][2] + m1.m[0][1] * m1.m[1][2] * m1.m[3][0] + m1.m[0][2] * m1.m[1][0] * m1.m[3][1] - m1.m[0][2] * m1.m[1][1] * m1.m[3][0] - m1.m[0][0] * m1.m[1][2] * m1.m[3][1] - m1.m[0][1] * m1.m[1][0] * m1.m[3][2]);
	m2.m[3][3] = invDet * (m1.m[0][0] * m1.m[1][1] * m1.m[2][2] + m1.m[0][1] * m1.m[1][2] * m1.m[2][0] + m1.m[0][2] * m1.m[1][0] * m1.m[2][1] - m1.m[0][2] * m1.m[1][1] * m1.m[2][0] - m1.m[0][0] * m1.m[1][2] * m1.m[2][1] - m1.m[0][1] * m1.m[1][0] * m1.m[2][2]);

	return m2;
}

Matrix4x4 MakeIdentity4x4(){
	Matrix4x4 m1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m1.m[i][j] = 0.0f;
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				m1.m[i][j] = 1.0f;
			}
		}
	}

	return m1;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				re.m[i][j] = 1.0f;
			}

		}
	}

	re.m[3][0] = translate.x;
	re.m[3][1] = translate.y;
	re.m[3][2] = translate.z;;


	return re;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				re.m[i][j] = 1.0f;
			}

		}
	}

	re.m[0][0] = scale.x;
	re.m[1][1] = scale.y;
	re.m[2][2] = scale.z;


	return re;
}

Vector3 TransformV3ToM4x4(const Vector3& vector, Matrix4x4 matrix){
	Vector3 re;

	re.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];

	re.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];

	re.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];

	float w;
	w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

	re.x /= w;
	re.y /= w;
	re.z /= w;

	return re;
}

Matrix4x4 MakeRotateXMatrix(float radian){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				re.m[i][j] = 1.0f;
			}
		}
	}

	re.m[1][1] = cosf(radian);
	re.m[2][2] = cosf(radian);
	re.m[1][2] = sinf(radian);
	re.m[2][1] = -sinf(radian);

	return re;
}

Matrix4x4 MakeRotateYMatrix(float radian){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				re.m[i][j] = 1.0f;
			}
		}
	}

	re.m[0][0] = cosf(radian);
	re.m[2][2] = cosf(radian);
	re.m[2][0] = sinf(radian);
	re.m[0][2] = -sinf(radian);

	return re;
}

Matrix4x4 MakeRotateZMatrix(float radian){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j) {
				re.m[i][j] = 1.0f;
			}
		}
	}

	re.m[0][0] = cosf(radian);
	re.m[1][1] = cosf(radian);
	re.m[1][0] = -sinf(radian);
	re.m[0][1] = sinf(radian);

	return re;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate){
	Matrix4x4 re;

	Matrix4x4 S;
	S = MakeScaleMatrix(scale);

	Matrix4x4 Rxyz;
	Matrix4x4 Rx, Ry, Rz;
	Rx = MakeRotateXMatrix(rotate.x);
	Ry = MakeRotateYMatrix(rotate.y);
	Rz = MakeRotateZMatrix(rotate.z);
	Rxyz = Multiply(Rx, Multiply(Ry, Rz));

	Matrix4x4 T;
	T = MakeTranslateMatrix(translate);

	re = Multiply(S, Multiply(Rxyz, T));

	return re;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip){
	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}
	re.m[2][3] = 1.0f;

	re.m[0][0] = (1.0f / aspectRatio) * (1.0f / std::tanf(fovY / 2.0f));
	re.m[1][1] = 1.0f / std::tanf(fovY / 2.0f);
	re.m[2][2] = farClip / (farClip - nearClip);
	re.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);




	return re;
}
