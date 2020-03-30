#pragma once

class CollisionScene : public Scene
{
private:
	Collider* coll;
	Collider* coll2;
public:
	CollisionScene();
	~CollisionScene();
		
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};