//#pragma once
//
//class ModelRender
//{
//private:
//	wstring shaderFile;
//
//	Model* model;
//
//	Matrix bones[MAX_MODEL_BONE];
//	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_BONE];
//
//	vector<Transform*> transforms;
//	Matrix worlds[MAX_MODEL_INSTANCE];
//
//	VertexBuffer* instanceBuffer;
//
//	ID3D11Texture2D* texture;
//	ID3D11ShaderResourceView* srv;
//
//public:
//	ModelRender(wstring shaderFile);
//	~ModelRender();
//
//	void Update();
//	void Render();
//
//	void UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform);
//	void UpdateTransforms();
//
//	void ReadMaterial(string file);
//	void ReadMesh(string file);
//
//	Transform* AddTransform();
//	Transform* GetTransform(UINT index) { return transforms[index]; }
//
//	Model* GetModel() { return model; }
//private:
//	void UpdateBones(ModelBone* bone, Matrix& matrix);
//
//	void CreateTexture();
//};
#pragma once

class ModelRender
{
private:
	wstring shaderFile;

	Model* model;

	Matrix bones[MAX_MODEL_BONE];
	Matrix boneTransforms[MAX_MODEL_INSTANCE][MAX_MODEL_BONE];

	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	// 추가한 건 여기에다가 넣었습니다
	string modelName;
	vector<BoxCollider*> totalCollBox;
	UINT curObjNum;
	UINT instanceNum;

public:
	ModelRender(wstring shaderFile);
	~ModelRender();

	void Update();
	void Render();

	void UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform);
	void UpdateTransforms();

	void ReadMaterial(string file);
	void ReadMesh(string file);

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }

	Model* GetModel() { return model; }

	// 추가
	string GetModelName() { return modelName; }
	void SetModelName(string value) { modelName = value; }

	BoxCollider* AddCollBox();
	vector<BoxCollider*> GetCollBox() { return totalCollBox; }

	void SetObjectNum(UINT value) { curObjNum = value; }
	UINT SetInstanceNum() { return instanceNum++; }
	UINT GetObjectNum() { return curObjNum; }
	UINT GetInstanceNum() { return instanceNum; }
	wstring GetShaderName() { return shaderFile; }

private:
	void UpdateBones(ModelBone* bone, Matrix& matrix);
	void CreateTexture();
};