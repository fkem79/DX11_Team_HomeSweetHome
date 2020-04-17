#pragma once

class FrustumScene : public Scene
{
private:
	Frustum* frustum;
	/*
	vector<Sphere*> spheres;
	UINT drawCount;
	Frustum* frustum;
	*/
	TerrainData* data;
	QuadTreeTerrain* terrain;
public:
	FrustumScene();
	~FrustumScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};