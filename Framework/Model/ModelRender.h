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

	UINT modelNum;
	BoxCollider* coll;
	bool boxRenderCheck;
	string modelName;
	string shaderName;
public:
	ModelRender(string name);
	ModelRender(string name, wstring shaderName);
	~ModelRender();

	void Update();
	void PreRender();
	void Render();
	void PostRender();

	void UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform);
	void UpdateTransforms();

	void ReadMaterial(string file);
	void ReadMesh(string file);

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }

	Model* GetModel() { return model; }
	void SetModelNum(UINT value) { modelNum = value; }
	UINT GetModelNum() { return modelNum; }

	BoxCollider* GetCollBox() { return coll; }
	void SetBoxRenderCheck(bool value) { boxRenderCheck = value; }
	string GetModelName() { return modelName; }
	string GetShaderName() { return shaderName; }

	void CreateCollBox();
	void Export(string name);
private:
	void UpdateBones(ModelBone* bone, Matrix& matrix);

	void CreateTexture();
};