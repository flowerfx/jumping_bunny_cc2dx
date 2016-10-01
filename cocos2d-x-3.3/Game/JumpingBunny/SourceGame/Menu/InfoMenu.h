#ifndef __INFO_MENU_H__
#define __INFO_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class InfoMenu : public MenuEntity
{
	InfoMenu();
	~InfoMenu();
	static InfoMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
public:
	static InfoMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new InfoMenu();
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
};
#endif //__INFO_MENU_H__