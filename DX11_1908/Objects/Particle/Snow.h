#pragma once
#include "Particle.h"

class Snow : public Particle
{
private:
	class SnowBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 color;

			Float3 velocity;
			float drawDistance;

			Float3 origin;
			float turbulence;

			Float3 extent;
			float time;
		}data;

		SnowBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.color = Float4(1, 1, 1, 1);

			data.velocity = Float3(0, -5, 0);
			data.drawDistance = 50.0f;

			data.origin = Float3(0, 0, 0);
			data.turbulence = 5.0f;

			data.extent = Float3(10, 10, 10);
			data.time = 0.0f;
		}
	};

	struct VertexSnow
	{
		Float3 position;
		float scale;
		Float2 random;
	};

	SnowBuffer* snowBuffer;
	vector<VertexSnow> vertices;
public:
	Snow();
	~Snow();

	void Update() override;
	void Render() override;

	void Create() override;
};