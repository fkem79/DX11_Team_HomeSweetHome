#pragma once
#include "Framework.h"

class ViewProjectionBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
	}data;

public:
	ViewProjectionBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.view = XMMatrixIdentity();
		data.projection = XMMatrixIdentity();
		data.invView = XMMatrixIdentity();
	}

	void SetView(Matrix value)
	{
		data.view = XMMatrixTranspose(value);
		data.invView = XMMatrixTranspose(XMMatrixInverse(nullptr, value));
	}

	void SetProjection(Matrix value)
	{
		data.projection = XMMatrixTranspose(value);
	}
};

class WorldBuffer : public ConstBuffer
{
private:
	struct Data
	{
		Matrix world;
	}data;

public:
	WorldBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.world = XMMatrixIdentity();		
	}

	void SetWorld(Matrix value)
	{
		data.world = XMMatrixTranspose(value);
	}
};

class ColorBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float4 color;
	}data;

	ColorBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.color = Float4(1, 1, 1, 1);
	}
};

class LightBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float3 direction;

		float specExp;

		Float4 ambient;		
		Float4 ambientFloor;		
	}data;

	LightBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.direction = Float3(0, -1, 0);
		data.specExp = 64.0f;
		data.ambient = Float4(0.1f, 0.1f, 0.1f, 1.0f);
		data.ambientFloor = Float4(0.1f, 0.1f, 0.1f, 1.0f);
	}
};

class IntBuffer : public ConstBuffer
{
public:
	struct Data
	{
		int indices[4];
	}data;

	IntBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (int& i : data.indices)
			i = 0;
	}
};

class FloatBuffer : public ConstBuffer
{
public:
	struct Data
	{
		float indices[4];
	}data;

	FloatBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		for (float& i : data.indices)
			i = 0.0f;
	}
};

class ShadowBuffer : public ConstBuffer
{
public:
	struct Data
	{
		Float2 mapSize;
		float bias;
		int quality;
	}data;

	ShadowBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.mapSize = Float2(0, 0);
		data.bias = -0.0006f;
		data.quality = 0;
	}
};

struct LightInfo
{
	enum Type
	{
		DIRECTION,
		POINT,
		SPOT,
		CAPSULE
	};

	Float4 color;
	Float3 position;
	float range;

	Float3 direction;

	float outer;
	float inner;

	float length;

	Type type;

	float padding;

	LightInfo()
	{
		type = POINT;

		color = Float4(1, 1, 1, 1);
		position = Float3(0, 0, 0);
		range = 80.0f;

		direction = Float3(0, -1, 0);
		outer = 65;
		inner = 55;

		length = 50;
	}
};

#define MAX_LIGHT 10

class LightInfoBuffer : public ConstBuffer
{
public:
	struct Data
	{
		LightInfo lights[MAX_LIGHT];
		UINT lightCount;

		float padding[3];
	}data;

	LightInfoBuffer() : ConstBuffer(&data, sizeof(Data))
	{
		data.lightCount = 0;
	}

	void Add(LightInfo light)
	{
		data.lights[data.lightCount] = light;
		data.lightCount++;
	}
};

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