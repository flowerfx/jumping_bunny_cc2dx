#pragma once
#include "GamePlay/APManager.h"
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "cocos2d.h"
#include "../../RKUtils/Code/RKThread_Code/RKThread.h"
class MainSceneGame : public Layer
{
private:
	static MainSceneGame * p_Instance;
	CustomCommand _customCommand;

	bool p_IsInitialized;

	Sprite * p_BGLoading;
	Label  * p_loadingText;
#if USE_SKY_MENU
	RKUtils::RKThread * p_threadUpdateEffect;
	static uint32 HandleThreadEffect(void* pThreadData);
	float p_deltaTime;
	bool p_effectUpdateFinish;
#endif


	SpriteBatchNode * p_spriteBatch;
public:
	// implement the "static node()" method manually
	CREATE_FUNC(MainSceneGame);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	//virtual bool update();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	//
	static MainSceneGame * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new MainSceneGame();
		}
		return p_Instance;
	}
	MainSceneGame();
	~MainSceneGame();
	//
	void EndScence();
	//update and main method
	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) ;
	void OnDraw(const Mat4&transform, uint32_t flags);
	//method with cocos2d engine
	void InitForFirstScence(Director * d)
	{
		d->runWithScene(MainSceneGame::scene());
		//
	}

};
