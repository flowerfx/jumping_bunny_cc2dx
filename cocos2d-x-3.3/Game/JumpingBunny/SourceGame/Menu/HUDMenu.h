#ifndef __HUD_MENU_H__
#define __HUD_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

#ifdef _DEBUG
//#define USE_CHEAT_BTN
#endif

class HUDMenu : public MenuEntity
{
	HUDMenu();
	~HUDMenu();
	static HUDMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	MENULAYER p_MenuReturnTo;

	Vec2 p_OriginScaleTimeCD;
	float p_DeltaScaleTimeCD;
	Vec2 p_ScaleDistanceTimeCD;
	float p_ScaleMaxTimeCD;
	float p_DirectScaleTimeCD;
	float p_OnDelayTime;
public:
	static HUDMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new HUDMenu();
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
	void SetMenuReturnTo(MENULAYER m) { p_MenuReturnTo = m; }
	void SetMenuSwithTo(MENULAYER m) { p_MenuToSwitchTo = m; }
	//handle behavious when interrupt
	void OnShowIGM() { OnProcessButton("PauseBtn"); }
	//
	void OnCabrivableNumberBoostItem();
	//
	void OnCheckScoreWithScoreBoard();
	//
	void ResetScaleCDTimer();
};
#endif //__HUD_MENU_H__