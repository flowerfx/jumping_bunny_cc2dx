#ifndef __OPTIONSMENU_H__
#define __OPTIONSMENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"

class OptionsMenu : public MenuEntity
{
private:
	static OptionsMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	int idxFbBtn;

	void SetColorFBBtn();
public:
	OptionsMenu();
	~OptionsMenu();
	static OptionsMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new OptionsMenu();
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
private:

	bool IsSoundEnable;
};
#endif //__OPTIONSMENU_H__