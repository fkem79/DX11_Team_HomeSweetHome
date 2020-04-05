#pragma once

class AStarScene : public Scene
{
private:
	Terrain* terrain;
	Kaya* kaya;
	CubeSky* sky;
	AStar* aStar;
public:
	AStarScene();
	~AStarScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};