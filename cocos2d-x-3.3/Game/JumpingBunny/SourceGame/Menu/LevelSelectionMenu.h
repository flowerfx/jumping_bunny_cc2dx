#ifndef __LEVELSELECTIONMENU_H__
#define __LEVELSELECTIONMENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

enum STATE_MOVE_LEVEL : unsigned int
{
	LEVEL_IDLE = 0,
	LEVEL_MOVE_LEFT,
	LEVEL_MOVE_RIGHT
};

class LevelSelectionDetail : public Ref
{
private:
	Sprite*								p_MainBackground;
	Sprite*								p_Star_0_bg;
	Sprite*								p_Star_1_bg;
	Sprite*								p_Star_2_bg;
	Sprite*								p_Star_3_bg;
	Sprite*								p_StateLock;

	StringEntity*						p_DisplayTime;
	StringEntity*						p_NumberIdx;


	Vec2								p_DeltaPosNumberIdx;
	Vec2								p_DeltaPosDisplayTime;
	Vec2								p_DeltaPosStarBG;

	float								p_TimePlay;
	int									p_CurrentIdx;
	int									p_CurentStarGain;
	bool								p_IsUnlock;

	bool								p_IsPress;
	Vec2								p_ScaleNormal;

	float								p_DistanceMove;
public:
	LevelSelectionDetail();
	~LevelSelectionDetail();

	void Init(int idx, Vec2 Pos);

	void Update(float dt);
	void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch_0 = NULL, SpriteBatchNode * spriteBatch_1 = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch_0 = NULL, SpriteBatchNode * spriteBatch_1 = NULL);

	void SetPosition(Point pos);
	Point GetPosition();

	Size GetNormalSize() { 
		Size s(0, 0);
		s = p_MainBackground->getContentSize() * p_MainBackground->getScale();
		return s;
	}

	void SetUnlock() { p_IsUnlock = true; }
	bool IsUnlock() { return p_IsUnlock; }
	void SetStarGain(int p) { p_CurentStarGain = p; }
	void SetTimePlay(float t) { 
		p_TimePlay = t; 
		p_DisplayTime->SetStringName(convertToDisplayTime(p_TimePlay));
	}

	bool IsPress() { return p_IsPress; }
	void IsPress(bool b) { p_IsPress = b; }

	int GetStarGain() { return p_CurentStarGain; }
	float GetTimePlay() { return p_TimePlay; }

	void UpdatePress()
	{
		//CCASSERT(p_Resource, "button invaid");
		Point s = p_ScaleNormal;
		if (p_IsPress)
		{
			s = p_ScaleNormal * 3.f / 4.f;
		}
		p_MainBackground->setScale(s.x);
	}

	Texture2D * GetAtlasFont(int idx);

	//
	void reloadString();

	//
	float GetDistanceMove() { return p_DistanceMove; }
	void  SetDistanceMove(float p) { p_DistanceMove = p; }
};

struct SESSION_STATE
{
	bool IsUnlock;
	int	 CurrentStar;
public:
	SESSION_STATE()
	{
		IsUnlock = false;
		CurrentStar = 0;
	}
	SESSION_STATE(bool b, int star)
	{
		IsUnlock = b;
		CurrentStar = star;
	}
};

class LevelSelectionMenu : public MenuEntity
{
private:
	static LevelSelectionMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	int idxFbBtn;
	std::thread p_threadCreateGame;
	bool IsFinishCreate;
	//
	Vector<LevelSelectionDetail*> p_ListLevel;
	//
	int									p_CurrentLevelPageIdx;
	STATE_MOVE_LEVEL					p_CurrentState;

	std::vector<SESSION_STATE*>			p_currentSession;

	float p_timeQuickMove;
	float p_originTimeQuickMove;
	float p_DeltaTimeQuickMove;


	SpriteBatchNode * p_font_1;
	SpriteBatchNode * p_font_2;

public:
	LevelSelectionMenu();
	~LevelSelectionMenu();
	static LevelSelectionMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new LevelSelectionMenu();
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

	void OnCheckPressWithLevel();
	// a selector callback
	virtual void OnProcessButton(std::string name);
	//
	void SetTimeQuickMove();
	//
	void CreateInitGamePlay();
	//
	void OnUpdatePress();

	//
	void OnProcessLevelIdx(int idx);
	//
	int OnUnlockLevel(int idx);
	//
	bool OnSetDetailAtLevel(int idx, int star, int time);
	//
	void OnReloadString() override;
	//
	void SetStringSession();
	void UnlockSession(int idx, bool b);
	void AddStarSession(int idx, int star);
	void InitSession(bool b = false, int star = 0);
	bool IsSessionUnlock(int idx);
};
#endif //__LEVELSELECTIONMENU_H__