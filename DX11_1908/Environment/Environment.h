#pragma once

class Environment
{
private:
	Matrix projection;

	ViewProjectionBuffer* viewProjection;
	LightBuffer* light;

	ID3D11SamplerState* linearSamp;
	ID3D11SamplerState* pointSamp;

	Camera* mainCamera;

	static Environment* instance;

	Environment();
	~Environment();
public:
	static Environment* Get() { return instance; }
	static void Create() { instance = new Environment(); }
	static void Delete() { delete instance; }

	void CreateViewport();
	void CreatePerspective();
	void CreateSamplerState();	

	ViewProjectionBuffer* GetVP() { return viewProjection; }
	Camera* MainCamera() { return mainCamera; }
	LightBuffer* GetLight() { return light; }
	Matrix GetProjection() { return projection; }
};