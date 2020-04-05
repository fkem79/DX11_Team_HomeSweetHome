#pragma once
#include "Framework.h"
//VertexLayouts

struct Vertex
{
	Float3 position;

	Vertex()
		: position(0, 0, 0)
	{		
	}
	Vertex(float x, float y, float z)
		: position(x, y, z)
	{
	}
};

struct VertexSize
{
	Float3 position;
	Float2 size;

	VertexSize()
		: position(0, 0, 0), size(0, 0)
	{
	}
};

struct VertexUV
{
	Float3 position;
	Float2 uv;

	VertexUV()
		: position(0, 0, 0), uv(0, 0)
	{
	}
};

struct VertexColor
{
	Float3 position;
	Float4 color;

	VertexColor()
		: position(0, 0, 0), color(1, 1, 1, 1)
	{
	}
};

struct VertexNormal
{
	Float3 position;
	Float3 normal;

	VertexNormal()
		: position(0, 0, 0), normal(0, 0, 0)
	{
	}
};

struct VertexUVNormal
{
	Float3 position;
	Float2 uv;
	Float3 normal;

	VertexUVNormal()
		: position(0, 0, 0), uv(0, 0), normal(0, 0, 0)
	{
	}
};

struct VertexUVNormalTangent
{
	Float3 position;
	Float2 uv;
	Float3 normal;
	Float3 tangent;

	VertexUVNormalTangent()
		: position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0)
	{
	}
};

struct VertexUVNormalTangentBlend
{
	Float3 position;
	Float2 uv;
	Float3 normal;
	Float3 tangent;
	Float4 blendIndices;
	Float4 blendWeights;

	VertexUVNormalTangentBlend()
		: position(0, 0, 0), uv(0, 0), normal(0, 0, 0), tangent(0, 0, 0),
		blendIndices(0, 0, 0, 0), blendWeights(0, 0, 0, 0)
	{
	}
};