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
private:
	void UpdateBones(ModelBone* bone, Matrix& matrix);

	void CreateTexture();
};