#pragma once

class RadialBlur : public Render2D
{
private:
	class BlurBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int count;
			float radius;
			float amount;

			float padding;
		}data;

		BlurBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.count = 1;
			data.radius = 0.0f;
			data.amount = 0.0f;			
		}
	};

	BlurBuffer* buffer;
public:
	RadialBlur();
	~RadialBlur();

	void Render();
};