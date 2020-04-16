#pragma once

class MapScene : public Scene
{
private:
	Belle* belle;
	Player* player;
	ModelSingle* tile1;
	ObjectCreateManager* ocm;

public:
	MapScene();
	~MapScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
