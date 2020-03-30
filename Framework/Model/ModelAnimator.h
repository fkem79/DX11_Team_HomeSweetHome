#pragma once

class ModelAnimator : public Transform
{
private:
	struct KeyFrameDesc
	{
		int clip = 0;

		UINT curFrame = 0;
		UINT nextFrame = 0;

		float time = 0.0f;
		float runningTime = 0.0f;

		float speed = 1.0f;

		float padding[2];
	};

	struct TweenDesc
	{
		float takeTime;
		float tweenTime;
		float runningTime;
		float padding;

		KeyFrameDesc cur;
		KeyFrameDesc next;

		TweenDesc()
		{
			takeTime = 1.0f;
			tweenTime = 0.0f;
			runningTime = 0.0f;

			cur.clip = 0;
			next.clip = -1;
		}
	};

	class FrameBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			TweenDesc tweenDesc[MAX_MODEL_INSTANCE];
		}data;

		FrameBuffer() : ConstBuffer(&data, sizeof(Data))
		{			
		}
	};

	struct ClipTransform
	{
		Matrix** transform;

		ClipTransform()
		{
			transform = new Matrix * [MAX_MODEL_KEY];

			for (UINT i = 0; i < MAX_MODEL_KEY; i++)
				transform[i] = new Matrix[MAX_MODEL_BONE];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEY; i++)
				delete[] transform[i];

			delete[] transform;
		}
	};

	wstring shader;
	Model* model;

	FrameBuffer* frameBuffer;
	ClipTransform* clipTransform;

	vector<Transform*> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;

	map<UINT, Event> EndEvent;
public:
	ModelAnimator(wstring shaderFile);
	~ModelAnimator();

	void Update();
	void Render();

	void ReadMaterial(string file);
	void ReadMesh(string file);
	void ReadClip(string file);

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 1.0f);

	Transform* AddTransform();

	void UpdateTransforms();

	Model* GetModel() { return model; }

	void SetEndEvent(UINT clip, Event value) { EndEvent[clip] = value; }
private:
	void CreateTexture();
	void CreateClipTransform(UINT index);
};