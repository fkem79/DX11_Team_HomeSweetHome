#include "Framework.h"

ModelMeshPart::ModelMeshPart()
{
	intBuffer = new IntBuffer();
	boneBuffer = new BoneBuffer();	
}

ModelMeshPart::~ModelMeshPart()
{
	delete intBuffer;
	delete boneBuffer;
	delete material;	
}

void ModelMeshPart::Update()
{
	boneBuffer->data.index = mesh->BoneIndex();		
}

void ModelMeshPart::Render()
{
	intBuffer->SetPSBuffer(10);
	boneBuffer->SetVSBuffer(2);
	
	material->Set();

	DC->DrawIndexed(indexCount, startIndex, 0);
}

void ModelMeshPart::Binding(Model* model)
{
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

void ModelMeshPart::SetShader(wstring shaderFile)
{
	material->SetShader(shaderFile);
}

void ModelMeshPart::SetTransforms(Matrix* transforms)
{
	memcpy(boneBuffer->data.transforms, transforms, sizeof(Matrix) * MAX_MODEL_BONE);
}
