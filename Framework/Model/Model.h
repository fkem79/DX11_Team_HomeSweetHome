#pragma once

class Model
{
private:
	ModelBone* root;

	map<string, Material*> materials;
	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;	
	vector<ModelClip*> clips;

public:
	Model();
	~Model();

	void ReadMaterial(string file);
	void ReadMesh(string file);
	void ReadClip(string file);

	void BindBone();
	void BindMesh();	

	void SetShader(wstring file);
	void SetShader(Shader* shader);

	void SetDiffuseMap(wstring file);
	void SetSpecularMap(wstring file);
	void SetNormalMap(wstring file);

	void SetSpecular(Float4 color);

	void Attach(wstring shader, Model* model,
		int parentBoneIndex, Transform* transform = nullptr);

	vector<ModelBone*>* GetBones() { return &bones; }
	vector<ModelMesh*>* GetMeshes() { return &meshes; }
	vector<ModelClip*>* GetClips() { return &clips; }

	UINT GetModelMeshSize() { return meshes.size(); }
//	UINT GetModelMeshVetexCount() { return meshes[0]->GetMeshVertexCount(); }
//	UINT GetModelMeshVetexCount1() { return meshes[1]->GetMeshVertexCount(); }
		
	Material* MaterialByName(string name) { return materials[name]; }
	ModelClip* ClipByIndex(UINT index) { return clips[index]; }
};