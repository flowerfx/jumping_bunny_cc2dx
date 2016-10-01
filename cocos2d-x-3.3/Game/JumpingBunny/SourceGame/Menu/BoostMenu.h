#ifndef __BOOST_MENU_H__
#define __BOOST_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"
#include "Utility/BoostManager.h"
class BoostMenu : public MenuEntity
{
	BoostMenu();
	~BoostMenu();
	static BoostMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	MENULAYER p_MenuReturnTo;

	bool p_isPopUpAppear;
public:
	static BoostMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new BoostMenu();
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
	//
	void OnCheckVisibleBtn();
	void OnCheckStatusBtn();
	//
	void OnParseDetailItem(BoostItem * b);
	//
	bool IsCanBuy(std::string name);
	bool IsCanUse(std::string name, bool IsShowMessage = true);
	void ProcessBuy(std::string name);
	void ProcessUse(std::string name, bool IsUSe = true);
	//callbackfunction
	static void CallBackFunction(void * p_Object, std::string str);
	void OnProcess(std::string str);
};
#endif //__BOOST_MENU_H__