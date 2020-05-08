#include "Framework.h"

ModelRender::ModelRender(wstring shaderFile)
	: shaderFile(shaderFile), modelNum(0), boxCollRenderCheck(true)
{
	model = new Model();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE, true);
	
}

ModelRender::~ModelRender()
{
	//for (BoxCollider* bc : totalCollBox)
	//	delete bc;

	//totalCollBox.clear();

	for (Transform* transform : transforms)
		delete transform;

	transforms.clear();

	texture->Release();
	srv->Release();

	delete instanceBuffer;
	delete model;
}

void ModelRender::Update()
{
	UpdateTransforms();

	for (auto mesh : *model->GetMeshes())
		mesh->Update();

	for (BoxCollider* bc : totalCollBox)
		bc->UpdateWorld();

	model->SetSpecular(Float4(0.0f, 0.0f, 0.0f, 1.0f));
}

void ModelRender::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->Set(1);

	for (auto mesh : *model->GetMeshes())
		mesh->Render(transforms.size());

	if (boxCollRenderCheck)
	{
		for (BoxCollider* bc : totalCollBox)
			bc->Render();
	}
}

void ModelRender::UpdateTransform(UINT instanceID, UINT boneIndex, Transform& transform)
{
	Matrix destMatrix = transform.GetWorld();

	ModelBone* bone = model->GetBones()->at(boneIndex);
	boneTransforms[instanceID][boneIndex] = destMatrix * boneTransforms[instanceID][boneIndex];

	int tempBoneIndex = boneIndex;
	for (ModelBone* child : bone->children)
	{
		Matrix parent = boneTransforms[instanceID][boneIndex];

		Matrix invParent = XMMatrixInverse(nullptr, parent);
		tempBoneIndex++;

		Matrix temp = boneTransforms[instanceID][tempBoneIndex] * invParent;
		boneTransforms[instanceID][tempBoneIndex] = temp * destMatrix * parent;
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, boneTransforms, MAX_MODEL_BONE * MAX_MODEL_INSTANCE * sizeof(Matrix));
	}
	DC->Unmap(texture, 0);
}

void ModelRender::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		Matrix temp = XMMatrixTranspose(transforms[i]->GetWorld());
		memcpy(&worlds[i], &temp, sizeof(Matrix));
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(instanceBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, MAX_MODEL_INSTANCE * sizeof(Matrix));
	}
	DC->Unmap(instanceBuffer->GetBuffer(), 0);
}

void ModelRender::ReadMaterial(string file)
{
	model->ReadMaterial(file);
}

void ModelRender::ReadMesh(string file)
{
	model->ReadMesh(file);

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->SetShader(shaderFile);
}

Transform* ModelRender::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);
	
	BoxCollider* box = AddCollBox();
	box->SetParent(transform->GetWorldPointer());

	return transform;
}

BoxCollider* ModelRender::AddCollBox()
{
	Float3 min = { 0, 0, 0 };
	Float3 max = { 0, 0, 0 };

	for (ModelMesh* mesh : *model->GetMeshes())
	{
		for (int i = 0; i < mesh->GetMeshVertexCount(); i++)
		{
			ModelVertex* vertices = mesh->GetModelVertexInfo();

			if (min.x > vertices[i].position.x)
				min.x = vertices[i].position.x;
			if (max.x < vertices[i].position.x)
				max.x = vertices[i].position.x;

			if (min.y > vertices[i].position.y)
				min.y = vertices[i].position.y;
			if (max.y < vertices[i].position.y)
				max.y = vertices[i].position.y;

			if (min.z > vertices[i].position.z)
				min.z = vertices[i].position.z;
			if (max.z < vertices[i].position.z)
				max.z = vertices[i].position.z;
		}
	}

	BoxCollider* box = new BoxCollider(min, max);
	totalCollBox.push_back(box);

	return box;
}

void ModelRender::UpdateBones(ModelBone* bone, Matrix& matrix)
{
	//Matrix temp = bone->transform;
	//bone->transform = temp * matrix;
	bone->transform *= matrix;

	for (ModelBone* child : bone->children)
		UpdateBones(child, matrix);
}

void ModelRender::CreateTexture()
{
	{//Create Texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;
		desc.Height = MAX_MODEL_INSTANCE;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		{
			for (UINT b = 0; b < model->GetBones()->size(); b++)
			{
				ModelBone* bone = model->GetBones()->at(b);

				Matrix parent;
				int parentIndex = bone->parentIndex;

				if (parentIndex < 0)
					parent = XMMatrixIdentity();
				else
					parent = bones[parentIndex];

				Matrix matrix = bone->transform;
				bones[b] = parent;
				boneTransforms[i][b] = matrix * bones[b];
			}
		}

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = boneTransforms;
		subResource.SysMemPitch = MAX_MODEL_BONE * sizeof(Matrix);
		subResource.SysMemSlicePitch = MAX_MODEL_BONE * sizeof(Matrix) * MAX_MODEL_INSTANCE;

		V(DEVICE->CreateTexture2D(&desc, &subResource, &texture));
	}

	{//Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = 1;

		V(DEVICE->CreateShaderResourceView(texture, &desc, &srv));
	}

	for (ModelMesh* mesh : *model->GetMeshes())
	{
		mesh->SetTransformsSRV(srv);
	}
}
