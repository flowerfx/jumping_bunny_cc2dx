#ifndef __RESULT_MENU_H__
#define __RESULT_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

enum TAG_EFFECT_TEXT
{
	TEXT_ZOOM_EFFECT = 0,
};

enum TYPE_END_RESULT : int
{
	RESULT_NONE= 0,
	RESULT_DIE,
	RESULT_END_TIME,
	RESULT_PASS_H_SCORE,
	RESULT_PASS_LEVEL
};

struct TimeEffectStar
{
	Vec2	OriginScaleStar;
	float	DeltaScaleStar;
	Vec2	ScaleDistanceStar;
	float	DistanceScaleStar;

	bool	OnShow;
	TimeEffectStar()
	{
		OriginScaleStar = ScaleDistanceStar = Vec2(0, 0);
		DistanceScaleStar = 3.0f;
		DeltaScaleStar = 0.2f;
		OnShow = false;
	}
};

class ResultMenu : public MenuEntity
{
	ResultMenu();
	~ResultMenu();
	static ResultMenu * m_Instance;
	bool p_IsGainHScore;
	MENULAYER p_MenuToSwitchTo;

	TYPE_END_RESULT p_CurrentEndResult;

	Vec2 p_OriginScaleTitle;
	Vec2 p_DeltaScale;
	Vec2 p_ScaleDistance;
	bool IsFinishFadeIn;

	TimeEffectStar p_timeStar[3];
	bool IsStartShowStar;

	bool p_IsThreadFinish;
	bool p_IsContinueGameLevel;
	std::thread p_ThreadCreateGame;

	void OnCreateNextLevel();
public:
	static ResultMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new ResultMenu();
		}
		return m_Instance;
	}

	void Init();
	void Update(float dt);
	void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void OnFadeIn();
	void OnFadeOut();
	void OnShow();
	void OnHide();

	// a selector callback
	virtual void OnProcessButton(std::string name);
	void Process(std::string name);
	MENULAYER GetMenuSwithTo() { return p_MenuToSwitchTo; }

	void SetScoreGame(TYPE_END_RESULT result = TYPE_END_RESULT::RESULT_NONE);
	//callbackfunction
	static void CallBackFunction(void * p_Object, std::string str);
};
#endif //__RESULT_MENU_H__