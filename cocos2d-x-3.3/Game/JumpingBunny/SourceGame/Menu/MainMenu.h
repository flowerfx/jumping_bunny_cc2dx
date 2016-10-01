#ifndef __MAINMENU_H__
#define __MAINMENU_H__
#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"
#include "Background/BGObject.h"
class MainMenu : public MenuEntity
{
public:
	MainMenu();
	~MainMenu();
	static MainMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	MENULAYER p_MenuReturnTo;

	bool p_IsExitGame;

	Sprite * p_Avatar;
public:
	static MainMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new MainMenu();
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
	//
	void SetCoin();

	// a selector callback
	virtual void OnProcessButton(std::string name);
	// implement the "static node()" method manually
	void OnSetVisibleBtn(std::string name_btn, bool IsVisible);
	//
	void SetMenuReturnTo(MENULAYER m) { p_MenuReturnTo = m; }
	//callback popup
	static void CallBackFunction(void * p_Object, std::string str);
	//exit game
	void callExitGame();
	//
	bool IsExitGame() { return p_IsExitGame; }
	//
	void DisplayProfileFB(std::wstring name, std::string imgPath);
};
#endif

