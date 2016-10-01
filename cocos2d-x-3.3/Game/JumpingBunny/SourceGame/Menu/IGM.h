#ifndef __IG_MENU_H__
#define __IG_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class IGMenu : public MenuEntity
{
	IGMenu();
	~IGMenu();
	static IGMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
public:
	static IGMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new IGMenu();
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

	void OnProcess(std::string name);

	MENULAYER GetMenuSwithTo() { return p_MenuToSwitchTo; }

	//callbackfunction
	static void CallBackFunction(void * p_Object, std::string str);
};
#endif //__IG_MENU_H__