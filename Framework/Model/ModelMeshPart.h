#pragma once

struct ModelBone
{
	int index;
	string name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> children;
};

class Model;
class ModelMesh;

class ModelMeshPart
{
public:
	friend class Model;
	friend class ModelMesh;

	class BoneBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Matrix transforms[MAX_MODEL_BONE];

			UINT index;
			float padding[3];			
		}data;

		BoneBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			for (UINT i = 0; i < MAX_MODEL_BONE; i++)
				data.transforms[i] = XMMatrixIdentity();

			data.index = 0;
		}
	};

private:
	string name;

	IntBuffer* intBuffer;
	BoneBuffer* boneBuffer;	

	ModelMesh* mesh;

	string materialName;
	Material* material;

	UINT startVertex;
	UINT startIndex;

	UINT vertexCount;
	UINT indexCount;

	ModelMeshPart();
	~ModelMeshPart();

	void Update();
	void Render();

	void Binding(Model* model);

	void SetShader(wstring shaderFile);
	void SetTransforms(Matrix* transforms);
};