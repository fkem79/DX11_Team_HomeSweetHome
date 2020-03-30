#pragma once

class ModelReader
{
private:
	string file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<AssimpMaterial*> materials;
	vector<AssimpBone*> bones;
	vector<AssimpMesh*> meshes;

public:
	ModelReader();
	~ModelReader();

	void ReadFile(string file);

	void ExportMaterial(string savePath, bool isOverwrite = true);

	void ReadMaterial();
	void WriteMaterial(string savePath, bool isOverwrite);
	string WriteTexture(string savePath, string file);

	void ExportMesh(string savePath, bool isOverwrite = true);

	void ReadBoneData(aiNode* node, int index, int parent);
	void ReadMeshData(aiNode* node, int bone);
	void ReadSkinData();
	void WriteMeshData(string savePath, bool isOverwrite);

	vector<string> GetClipList();
	void ExportAnimClip(UINT index, string savePath, bool isOverwrite = true);

	Clip* ReadClipData(aiAnimation* animation);
	void ReadKeyFrameData(Clip* clip, aiNode* node, vector<ClipNode>& nodeInfos);
	void WriteClipData(Clip* clip, string savePath, bool isOverwrite);
};