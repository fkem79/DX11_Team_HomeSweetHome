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

<<<<<<< HEAD
	void SetSpecular(Float4 color);

=======
>>>>>>> sub/master
	void Attach(wstring shader, Model* model,
		int parentBoneIndex, Transform* transform = nullptr);

	vector<ModelBone*>* GetBones() { return &bones; }
	vector<ModelMesh*>* GetMeshes() { return &meshes; }
	vector<ModelClip*>* GetClips() { return &clips; }
		
	Material* MaterialByName(string name) { return materials[name]; }
	ModelClip* ClipByIndex(UINT index) { return clips[index]; }
};