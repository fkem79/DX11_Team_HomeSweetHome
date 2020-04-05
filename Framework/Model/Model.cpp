#include "Framework.h"
#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	for (auto material : materials)
		delete material.second;

	for (auto bone : bones)
		delete bone;

	for (auto mesh : meshes)
		delete mesh;

	for (auto clip : clips)
		delete clip;
}

void Model::ReadMaterial(string file)
{
	file = "ModelData/Materials/" + file + ".mat";

	wstring folder = Path::ToWString(Path::GetDirectoryName(file));

	XmlDocument* document = new XmlDocument();
	document->LoadFile(file.c_str());

	XmlElement* root = document->FirstChildElement();
	XmlElement* matNode = root->FirstChildElement();

	do
	{
		XmlElement* node = matNode->FirstChildElement();

		Material* material = new Material();
		material->name = node->GetText();

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Path::ToWString(node->GetText());
			material->SetDiffuseMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Path::ToWString(node->GetText());
			material->SetSpecularMap(folder + file);
		}

		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring file = Path::ToWString(node->GetText());
			material->SetNormalMap(folder + file);
		}

		Float4 color;

		node = node->NextSiblingElement();		
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.ambient = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.diffuse = color;

		node = node->NextSiblingElement();
		color.x = node->FloatAttribute("R");
		color.y = node->FloatAttribute("G");
		color.z = node->FloatAttribute("B");
		color.w = node->FloatAttribute("A");
		material->GetBuffer()->data.specular = color;

		materials.insert({ material->name, material });

		matNode = matNode->NextSiblingElement();
	} while (matNode != nullptr);

	delete document;
}

void Model::ReadMesh(string file)
{
	file = "ModelData/Meshes/" + file + ".mesh";
	BinaryReader* r = new BinaryReader(file);

	UINT count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = r->String();
		bone->parentIndex = r->Int();
		bone->transform = XMLoadFloat4x4(&r->Matrix());

		bones.push_back(bone);
	}

	count = r->UInt();

	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();
		mesh->name = r->String();
		mesh->boneIndex = r->Int();

		mesh->materialName = r->String();

		{//VertexData
			UINT count = r->UInt();

			mesh->vertexCount = count;
			mesh->vertices = new ModelVertex[count];

			void* ptr = (void*)mesh->vertices;
			r->Byte(&ptr, sizeof(ModelVertex) * count);
		}

		{//IndexData
			UINT count = r->UInt();

			mesh->indexCount = count;
			mesh->indices = new UINT[count];

			void* ptr = (void*)mesh->indices;
			r->Byte(&ptr, sizeof(UINT) * count);
		}		

		meshes.push_back(mesh);
	}

	delete r;

	BindBone();
	BindMesh();
}

void Model::ReadClip(string file)
{
	file = "ModelData/Clips/" + file + ".clip";

	BinaryReader* r = new BinaryReader(file);

	ModelClip* clip = new ModelClip();

	clip->name = r->String();
	clip->duration = r->Float();
	clip->frameRate = r->Float();
	clip->frameCount = r->UInt();

	UINT keyFramesCount = r->UInt();
	for (UINT i = 0; i < keyFramesCount; i++)
	{
		KeyFrame* keyFrame = new KeyFrame();
		keyFrame->boneName = r->String();

		UINT size = r->UInt();
		if (size > 0)
		{
			keyFrame->transforms.resize(size);

			void* ptr = (void*)keyFrame->transforms.data();
			r->Byte(&ptr, sizeof(KeyTransform) * size);
		}

		clip->keyFrameMap[keyFrame->boneName] = keyFrame;
	}
	clips.push_back(clip);

	delete r;
}

void Model::BindBone()
{
	root = bones[0];

	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->children.push_back(bone);
		}
		else
			bone->parent = nullptr;
	}
}

void Model::BindMesh()
{
	for (auto mesh : meshes)
	{
		for (auto bone : bones)
		{
			if (mesh->boneIndex == bone->index)
			{
				mesh->bone = bone;
				break;
			}
		}

		mesh->Binding(this);
	}
}

void Model::SetShader(wstring file)
{
	for (ModelMesh* mesh : meshes)
		mesh->SetShader(file);
}

void Model::SetShader(Shader* shader)
{
	for (ModelMesh* mesh : meshes)
		mesh->SetShader(shader);
}

void Model::SetDiffuseMap(wstring file)
{
	for (ModelMesh* mesh : meshes)
		mesh->GetMaterial()->SetDiffuseMap(file);
}

void Model::SetSpecularMap(wstring file)
{
	for (ModelMesh* mesh : meshes)
		mesh->GetMaterial()->SetSpecularMap(file);
}

void Model::SetNormalMap(wstring file)
{
	for (ModelMesh* mesh : meshes)
		mesh->GetMaterial()->SetNormalMap(file);
}

void Model::SetSpecular(Float4 color)
{
	for (ModelMesh* mesh : meshes)
	{
		mesh->GetMaterial()->GetBuffer()->data.specular = color;
	}
}

void Model::Attach(wstring shader, Model* model, int parentBoneIndex, Transform* transform)
{
	//Copy Material
	for (auto material : model->materials)
	{
		Material* newMaterial = new Material(shader);

		newMaterial->name = material.second->name;
		newMaterial->GetBuffer()->data = material.second->GetBuffer()->data;

		if (material.second->GetDiffuseMap() != nullptr)
			newMaterial->SetDiffuseMap(material.second->GetDiffuseMap()->GetFile());

		if (material.second->GetSpecularMap() != nullptr)
			newMaterial->SetSpecularMap(material.second->GetSpecularMap()->GetFile());

		if (material.second->GetNormalMap() != nullptr)
			newMaterial->SetNormalMap(material.second->GetNormalMap()->GetFile());

		materials.insert({ newMaterial->name, newMaterial });
	}

	vector<pair<int, int>> changes;

	{//Copy Bone
		ModelBone* parentBone = bones[parentBoneIndex];

		for (ModelBone* bone : model->bones)
		{
			ModelBone* newBone = new ModelBone();
			newBone->name = bone->name;
			newBone->transform = bone->transform;

			if (transform != nullptr)
				newBone->transform = newBone->transform * transform->GetWorld();

			if (bone->parent != nullptr)
			{
				int parentIndex = bone->parentIndex;

				for (pair<int, int>& temp : changes)
				{
					if(temp.first == parentIndex)
					{
						newBone->parentIndex = temp.second;
						newBone->parent = bones[newBone->parentIndex];
						newBone->parent->children.push_back(newBone);

						break;
					}
				}
			}
			else
			{
				newBone->parentIndex = parentBoneIndex;
				newBone->parent = parentBone;
				newBone->parent->children.push_back(newBone);
			}

			newBone->index = bones.size();
			changes.push_back(pair<int, int>(bone->index, newBone->index));

			bones.push_back(newBone);
		}
	}

	{//Copy Mesh
		for (ModelMesh* mesh : model->meshes)
		{
			ModelMesh* newMesh = new ModelMesh();
			for (pair<int, int>& temp : changes)
			{
				if (temp.first == mesh->boneIndex)
				{
					newMesh->boneIndex = temp.second;
					break;
				}
			}

			newMesh->bone = bones[newMesh->boneIndex];
			newMesh->name = mesh->name;
			newMesh->materialName = mesh->materialName;

			newMesh->vertexCount = mesh->vertexCount;
			newMesh->indexCount = mesh->indexCount;

			UINT verticesSize = newMesh->vertexCount * sizeof(ModelVertex);
			newMesh->vertices = new ModelVertex[newMesh->vertexCount];
			memcpy_s(newMesh->vertices, verticesSize, mesh->vertices, verticesSize);

			UINT indicesSize = newMesh->indexCount * sizeof(UINT);
			newMesh->indices = new UINT[newMesh->indexCount];
			memcpy_s(newMesh->indices, indicesSize, mesh->indices, indicesSize);

			newMesh->Binding(this);
			newMesh->SetShader(shader);

			meshes.push_back(newMesh);
		}
	}
}
