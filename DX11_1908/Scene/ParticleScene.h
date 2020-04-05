#pragma once

class ParticleScene : public Scene
{
private:
	class Rain* rain;
	class Spark* spark;
	class Snow* snow;

	CubeSky* sky;
public:
	ParticleScene();
	~ParticleScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};