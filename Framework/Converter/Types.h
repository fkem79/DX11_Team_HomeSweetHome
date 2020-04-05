#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;

struct AssimpMaterial
{
	string name;

	Float4 ambient;
	Float4 diffuse;
	Float4 specular;

	string diffuseFile;
	string specularFile;
	string normalFile;
};

struct AssimpBone
{
	int index;
	string name;

	int parent;
	Matrix transform;
};

struct AssimpMeshPart
{
	string name;
	string materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};

struct AssimpMesh
{
	string name;
	int boneIndex;

	aiMesh* mesh;

	string materialName;

	vector<ModelVertex> vertices;
	vector<UINT> indices;	
};

struct AssimpBlendWeight
{
	Float4 indices = Float4(0, 0, 0, 0);
	Float4 weights = Float4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0:
			indices.x = i;
			weights.x = w;
			break;
		case 1:
			indices.y = i;
			weights.y = w;
			break;
		case 2:
			indices.z = i;
			weights.z = w;
			break;
		case 3:
			indices.w = i;
			weights.w = w;
			break;
		}
	}
};

struct AssimpBoneWeights
{
private:
	typedef pair<int, float> Pair;
	vector<Pair> boneWeights;

public:
	void AddWeights(UINT index, float weight)
	{
		if (weight <= 0.0f)
			return;

		bool isAdd = false;
		vector<Pair>::iterator it = boneWeights.begin();
		while (it != boneWeights.end())
		{
			if (weight > it->second)
			{
				boneWeights.insert(it, Pair(index, weight));
				isAdd = true;
				break;
			}
			it++;
		}

		if (isAdd == false)
			boneWeights.push_back(Pair(index, weight));
	}

	void GetBlendWeights(AssimpBlendWeight& blendWeight)
	{
		for (UINT i = 0; i < boneWeights.size(); i++)
		{
			if (i >= 4)
				return;

			blendWeight.Set(i, boneWeights[i].first, boneWeights[i].second);
		}
	}

	void Normalize()
	{
		float totalWeight = 0.0f;

		int i = 0;
		vector<Pair>::iterator it = boneWeights.begin();
		while (it != boneWeights.end())
		{
			if (i < 4)
			{
				totalWeight += it->second;
				i++;
				it++;
			}
			else
				it = boneWeights.erase(it);
		}

		float scale = 1.0f / totalWeight;

		it = boneWeights.begin();
		while (it != boneWeights.end())
		{
			it->second *= scale;
			it++;
		}
	}
};

struct KeyTransform
{
	float time;

	Float3 scale;
	Float4 rotation;
	Float3 position;
};

struct KeyFrame
{
	string boneName;
	vector<KeyTransform> transforms;
};

struct ClipNode
{
	vector<KeyTransform> keyFrame;
	aiString name;
};

struct Clip
{
	string name;

	UINT frameCount;
	float frameRate;
	float duration;

	vector<KeyFrame*> keyFrames;
};