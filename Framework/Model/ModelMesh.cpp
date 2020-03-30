#include "Framework.h"

ModelMesh::ModelMesh()
	: transformsSRV(nullptr), material(nullptr)
{	
	boneBuffer = new BoneBuffer();
	intBuffer = new IntBuffer();
}

ModelMesh::~ModelMesh()
{
	delete vertexBuffer;
	delete indexBuffer;

	delete[] vertices;
	delete[] indices;

	delete boneBuffer;
	delete intBuffer;
	delete material;
}

void ModelMesh::Binding(Model* model)
{
	vertexBuffer = new VertexBuffer(vertices, sizeof(ModelVertex), vertexCount);
	indexBuffer = new IndexBuffer(indices, indexCount);

	Material* srcMaterial = model->MaterialByName(materialName);

	material = new Material();
	material->GetBuffer()->data = srcMaterial->GetBuffer()->data;

	if (srcMaterial->GetDiffuseMap() != nullptr)
		material->SetDiffuseMap(srcMaterial->GetDiffuseMap()->GetFile());

	if (srcMaterial->GetSpecularMap() != nullptr)
	{
		material->SetSpecularMap(srcMaterial->GetSpecularMap()->GetFile());
		intBuffer->data.indices[0] = 1;
	}

	if (srcMaterial->GetNormalMap() != nullptr)
	{
		material->SetNormalMap(srcMaterial->GetNormalMap()->GetFile());
		intBuffer->data.indices[1] = 1;
	}
}

void ModelMesh::Update()
{
	boneBuffer->data.index = boneIndex;
}

void ModelMesh::Render(UINT drawCount)
{
	boneBuffer->SetVSBuffer(2);

	material->Set();

	if (transformsSRV != nullptr)
		DC->VSSetShaderResources(3, 1, &transformsSRV);

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	DC->DrawIndexedInstanced(indexCount, drawCount, 0, 0, 0);
}
/*
void ModelMesh::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();

	IASetPT();

	for (ModelMeshPart* part : meshParts)
		part->Render();
}*/