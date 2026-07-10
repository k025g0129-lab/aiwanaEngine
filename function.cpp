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

Matrix4x4 Transpose(const Matrix4x4& m1){

	Matrix4x4 m2;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m2.m[i][j] = m1.m[j][i];
		}
	}

	return m2;
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

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip){

	Matrix4x4 re;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			re.m[i][j] = 0.0f;
		}
	}
	re.m[3][3] = 1.0f;


	re.m[0][0] = 2.0f / (right - left);
	re.m[1][1] = 2.0f / (top - bottom);
	re.m[2][2] = 1.0f / (farClip - nearClip);
	re.m[3][0] = (left + right) / (left - right);
	re.m[3][1] = (top + bottom) / (bottom - top);
	re.m[3][2] = nearClip / (nearClip - farClip);



	return re;
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color){

	const uint32_t kSubdivision = 16;
	const float kLonEvery = 2.0f * static_cast<float>(M_PI) / static_cast<float>(kSubdivision);
	const float kLatEvery = static_cast<float>(M_PI) / static_cast<float>(kSubdivision);

	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++) {

		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++) {
			float lon = lonIndex * kLonEvery;

			Vector3 a, b, c, d;
			a.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon);
			a.y = sphere.center.y + sphere.radius * sinf(lat);
			a.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon);

			b.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon);
			b.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			b.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon);

			c.x = sphere.center.x + sphere.radius * cosf(lat) * cosf(lon + kLonEvery);
			c.y = sphere.center.y + sphere.radius * sinf(lat);
			c.z = sphere.center.z + sphere.radius * cosf(lat) * sinf(lon + kLonEvery);

			d.x = sphere.center.x + sphere.radius * cosf(lat + kLatEvery) * cosf(lon + kLonEvery);
			d.y = sphere.center.y + sphere.radius * sinf(lat + kLatEvery);
			d.z = sphere.center.z + sphere.radius * cosf(lat + kLatEvery) * sinf(lon + kLonEvery);

			Matrix4x4 vpv = Multiply(viewProjectionMatrix, viewportMatrix);

			Vector3 as = TransformV3ToM4x4(a, vpv);
			Vector3 bs = TransformV3ToM4x4(b, vpv);
			Vector3 cs = TransformV3ToM4x4(c, vpv);
			Vector3 ds = TransformV3ToM4x4(d, vpv);

			color = color;
			/*Novice::DrawLine(
				static_cast<int>(as.x), static_cast<int>(as.y),
				static_cast<int>(bs.x), static_cast<int>(bs.y),
				color);

			Novice::DrawLine(
				static_cast<int>(as.x), static_cast<int>(as.y),
				static_cast<int>(cs.x), static_cast<int>(cs.y),
				color);*/

		}

	}
}

ModelData LoadObjFile(const std::string& directoryPath, const std::string& filename){

	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file,line)){

		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v"){
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);

		} else if(identifier == "vt") {
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoords.push_back(texcoord);
			
		} else if (identifier == "vn") {
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);

		} else if(identifier == "f"){

			VertexData triangle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);

				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];

				//何故かxじゃなくてyを-1倍すると資料通りになる
				position.x *= -1.0f;
				
				//normal.x *= -1.0f;
				texcoord.y = 1.0f - texcoord.y;
				 
				//VertexData vertex = {position, texcoord, normal};
				//modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position,texcoord,normal };

			}
			modelData.vertices.push_back(triangle[2]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[0]);

		} else if(identifier == "mtllib") {
			std::string materialFilename;
			s >> materialFilename;

			modelData.material = LoadMaterialTemplateFile(directoryPath, materialFilename);
		}
	}
	return modelData;
}

MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename){

	//変数宣言
	MaterialData materialData;	
	std::string line;
	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	//materialData構築
	while (std::getline(file,line)){
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd"){
			std::string textureFilename;
			s >> textureFilename;

			materialData.textureFilrPath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}
