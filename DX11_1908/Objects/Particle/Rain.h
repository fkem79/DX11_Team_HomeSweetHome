#pragma once
#include "Particle.h"

class Rain : public Particle
{
private:
	class RainBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 velocity;
			float distance;

			Float4 color;
			Float3 origin;
			float time;

			Float3 size;

			float padding;
		}data;

		RainBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.velocity = Float3(0, -1, 0);
			data.distance = 1000.0f;

			data.color = Float4(1, 1, 1, 1);

			data.origin = Float3(0, 0, 0);
			data.size = Float3(0, 0, 0);

			data.time = 0.0f;
		}
	};

	RainBuffer* rainBuffer;
	vector<VertexSize> vertices;
	
public:
	Rain();
	~Rain();

	void Update() override;
	void Render() override;

	void Create() override;
};