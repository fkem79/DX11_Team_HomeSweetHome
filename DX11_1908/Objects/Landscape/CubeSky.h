#pragma once

class CubeSky
{
private:
	class Sphere* sphere;
	Texture* cubeMap;

	DepthStencilState* depthMode[2];

public:
	CubeSky();
	~CubeSky();

	void Render();
<<<<<<< HEAD
=======

	Texture* GetCubeMap() { return cubeMap; }
>>>>>>> sub/master
};