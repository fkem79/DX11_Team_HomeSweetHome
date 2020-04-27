
class StartScene : public Scene
{
private:
	Render2D* render2D;
	StartButton* start;
	EndButton* end;

	bool startgame;
public:
	StartScene();
	~StartScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Begin()override;
	void End()override;

	void Start();
	void Exit();

	void Texture2D();
};