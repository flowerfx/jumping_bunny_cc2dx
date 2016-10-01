#ifndef _MENU_MANAGER_H_
#define _MENU_MANAGER_H_
#pragma once
#include "cocos2d.h"
USING_NS_CC;
#include "Background/BGObject.h"
enum MENULAYER
{
	MENU_NONE = 0,
	MAINMENU,
	OPTIONMENU,
	INFOMENU,
	LOADMENU,
	IGM,
	RESULTMENU,
	ACTION_PHASE,
	IAP,
	BOOSTMENU,
	LEADERBOARD,
	GAMEPLAYSELECTMENU,
	LEVELSELECTIONMENU

};
class MenuManager
{
private:
	static MenuManager * m_Instance;
	MENULAYER  p_CurrentMenuLayer;
	BGObject * p_SkyObjectBG;
	int p_HScore;
	int p_Coin;
	bool p_IsInActionPhase;
	bool p_IsPopUpAppear;

	int p_StateLoadInit;
public:
	MenuManager();
	~MenuManager();
	MENULAYER GetCurrentMenuLayer()
	{
		return p_CurrentMenuLayer;
	}
	void  SetCurrentMenuLayer(MENULAYER m)
	{
		p_CurrentMenuLayer = m;
	}
	void  SwitchToMenu(MENULAYER m);
	void  CloseCurrentMenu(MENULAYER m);
	static MenuManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new MenuManager();
		}
		return m_Instance;
	}
	int Init();
	void Update(float dt);
	void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	//
	void SetHScore(int s, bool force = false);
	int GetHScore(bool force = false);
	//
	void SetCoin(int c, bool force = false);
	int Getcoin(bool force = false);
	//
	void IsInActionPhase(bool b) { p_IsInActionPhase = b; }
	bool IsInActionPhase() { return p_IsInActionPhase; }
	//
	void IsPopUpAppear(bool b) { p_IsPopUpAppear = b; }
	bool IsPopUpAppear() { return p_IsPopUpAppear; }
	//
	void OnUpdatePopUp(float dt);
	void OnShowPopUp(std::string str = "", int numberBtn = 2, std::string strCB = "", void * objectCallBack = NULL, void(*pt2Function)(void* pt2Object, std::string str) = NULL);
	void OnHidePopUp();
	//
	void OnReloadString();
	void HandleInterrupt();
	//
	void UpdateBG(float dt);
	void VisitBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void DrawBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void CheckAndRemoveBG();
	//
	int OnUnlockLevelIdx(int idx);
	bool OnSetDetailAtLevelIdx(int idx, int star, int time);
	bool IsThisSessionUnlock(int idx_level);
};

#define MenuMgr MenuManager::GetInstance()
#endif //_MENU_MANAGER_H_