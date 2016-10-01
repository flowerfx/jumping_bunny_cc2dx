#pragma once
#include "GamePlay/ManageObjectAP.h"
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "cocos2d.h"
#include "Background/BGObject.h"
#define USE_SKY_MENU 1
#define USE_CREATE_GAMEPLAY_IN_THREAD 1
#define LIMIT_TIME_RUN 30 //in second;
enum TYPE_BOOST
{
	B_NONE = 0,
	B_GRAVITY_BOOST,
	B_JUMP_BOOST,
	B_LIFE_BOOST
};
enum EVIROMENT_TYPE
{
	EV_NONE = 0,
	EV_DAYLIGHT,
	EV_SNOW,
	EV_RAIN,
	EV_NIGHT
};
enum TYPE_GAMEPLAY : int
{
	GAMEPLAY_ENDLESS_RUN = 0,
	GAMEPLAY_TIME_RUN,
	GAMEPLAY_PLAY_STORY
};
class APManager
{
private:
	static APManager * p_Instance;
	BGObject * p_SkyObject;
	EVIROMENT_TYPE	p_EviromentType;

	bool p_IsUseGravityBoost;
	bool p_IsUseJumpBoost;
	bool p_IsUseLifeBoost;

	TYPE_GAMEPLAY p_CurrentGamePlay;

	int p_currentPartPlay;
public:

	TYPE_GAMEPLAY GetCurrentGamePlay() { return p_CurrentGamePlay; }
	void SetCurrentGamePlay(TYPE_GAMEPLAY tp) { p_CurrentGamePlay = tp; }
	bool IsOnStoryMode() { return p_CurrentGamePlay == GAMEPLAY_PLAY_STORY; }

	int GetCurrentPartPlay() { return p_currentPartPlay; }
	void SetCurrentPartPlay(int tp) { p_currentPartPlay = tp; }

	std::string convertToPartName(int i)
	{
		return std::string("part_") + std::to_string(i);
	}
	std::string GetCurrentNameLevel()
	{
		return convertToPartName(p_currentPartPlay);
	}

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	bool init();

	void InitWithGamePlay(TYPE_GAMEPLAY tp);
	//
	static APManager * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new APManager();
		}
		return p_Instance;
	}
	APManager();
	~APManager();
	//
	//update and main method
	void update(float dt);
	void OnDraw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void OnVisit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	bool IsUseTheBoost(TYPE_BOOST b)
	{
		if (b == TYPE_BOOST::B_GRAVITY_BOOST)
		{
			return p_IsUseGravityBoost;
		}
		else if (b == TYPE_BOOST::B_JUMP_BOOST)
		{
			return p_IsUseJumpBoost;
		}
		else if (b == TYPE_BOOST::B_LIFE_BOOST)
		{
			return p_IsUseLifeBoost;
		}
		return false;
	}
	void SetUseTheBoost(TYPE_BOOST b, bool val)
	{
		if (b == TYPE_BOOST::B_GRAVITY_BOOST)
		{
			p_IsUseGravityBoost = val;
		}
		else if (b == TYPE_BOOST::B_JUMP_BOOST)
		{
			p_IsUseJumpBoost = val;
		}
		else if (b == TYPE_BOOST::B_LIFE_BOOST)
		{
			p_IsUseLifeBoost = val;
		}
	}
	void ResetAllBoost(bool ForceReset = true);

	EVIROMENT_TYPE GetCurrentEviType() { return p_EviromentType; }
};
#define APMgr APManager::GetInstance()