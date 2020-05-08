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
	UINT modelNum;
	vector<BoxCollider*> totalCollBox;
	bool boxCollRenderCheck;

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
	UINT GetModelNum() { return modelNum; }
	void SetModelName(string value) { modelName = value; }
	void SetModelNum(UINT value) { modelNum = value; }
	void SetBoxCollRenderCheck(bool value) { boxCollRenderCheck = value; }

	BoxCollider* AddCollBox();
	BoxCollider* GetCollBox(UINT index) { return totalCollBox[index]; }
	vector<BoxCollider*>* GetTotalCollBox() { return &totalCollBox; }
	vector<Transform*>* GetTransforms() { return &transforms; }

	int GetTransformsSize() { return transforms.size(); }
	int GetCollBoxSize() { return totalCollBox.size(); }
	bool GetBoxCollRenderCheck() { return boxCollRenderCheck; }

private:
	void UpdateBones(ModelBone* bone, Matrix& matrix);
	void CreateTexture();
};