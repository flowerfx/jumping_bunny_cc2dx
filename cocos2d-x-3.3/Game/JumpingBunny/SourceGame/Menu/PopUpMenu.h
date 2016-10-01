#ifndef __POPUP_MENU_H__
#define __POPUP_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class PopUpMenu : public MenuEntity
{
	PopUpMenu();
	~PopUpMenu();
	static PopUpMenu * m_Instance;
	std::string nameBtnProcess;
	//for the call back
	std::string p_strCallback;
	void* p_ObjectCallBack;
	void(* p_Function)(void * ptr, std::string str);
	int p_NumberBtn;
public:
	static PopUpMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new PopUpMenu();
		}
		return m_Instance;
	}

	void Init();
	void Update(float dt);
	void Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);
	void Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch = NULL);

	void OnFadeIn();
	void OnFadeOut();
	void OnShowPopUp(std::string str = "", int numberBtn = 2, std::string strCB = "", void * objectCallBack = NULL, void(*pt2Function)(void* pt2Object, std::string str) = NULL);
	void OnHide();

	// a selector callback
	virtual void OnProcessButton(std::string name);
	//void functionCallBack
	void callFunction(void(*pt2Function)(void* pt2Object, std::string str));
};

#define GetPopUp PopUpMenu::GetInstance()
#endif //__POPUP_MENU_H__