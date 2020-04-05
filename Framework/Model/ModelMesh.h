#pragma once

class Model;

class ModelMesh
{
private:
	friend class Model;

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

	string name;
	string materialName;

	int boneIndex;
	ModelBone* bone;	

	Material* material;

	BoneBuffer* boneBuffer;
	IntBuffer* intBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	ModelVertex* vertices;
	UINT* indices;

	UINT vertexCount, indexCount;

	ID3D11ShaderResourceView* transformsSRV;

	ModelMesh();
	~ModelMesh();

	void Binding(Model* model);

public:
	void Update();
	void Render(UINT drawCount);

	int BoneIndex() { return boneIndex; }

	void SetTransformsSRV(ID3D11ShaderResourceView* srv) { transformsSRV = srv; }
	void SetShader(wstring file) { material->SetShader(file); }
	void SetShader(Shader* shader) { material->SetShader(shader); }

	Material* GetMaterial() { return material; }
};