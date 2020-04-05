#pragma once
#include "Particle.h"

class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Float3 position;
		Float2 size;
		Float3 velocity;
	};

	class SparkBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			float duration;
			float time;
			float gravity;

			float padding;
		}data;

		SparkBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.duration = 0.0f;
			data.time = 0.0f;
			data.gravity = 0.0f;
		}
	};

	class ColorBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 start;
			Float4 end;
		}data;

		ColorBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.start = Float4(1, 0, 0, 1);
			data.end = Float4(1, 1, 0, 0.2f);
		}
	};

	SparkBuffer* sparkBuffer;
	ColorBuffer* colorBuffer;
	vector<VertexParticle> vertices;

public:
	Spark();
	~Spark();

	void Update() override;
	void Render() override;

	void Play(Vector3 position) override;

	void Create() override;
};