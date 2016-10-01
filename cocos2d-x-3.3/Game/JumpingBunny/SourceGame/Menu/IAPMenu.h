#ifndef __IAP_MENU_H__
#define __IAP_MENU_H__

#include "Menu/MenuEntity.h"
#include "Menu/MenuManager.h"
#include "Utility/IAPManager.h"
class IAPMenu : public MenuEntity
{
	IAPMenu();
	~IAPMenu();
	static IAPMenu * m_Instance;

	MENULAYER p_MenuToSwitchTo;
	MENULAYER p_MenuReturnTo;

	int p_idxItemProcess;
public:
	static IAPMenu * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new IAPMenu();
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
	void OnParseDetailIAPItem(IAPItem * b);
	//
	void SetPriceValue(std::string name, std::string value);
	//call back from IAP
	static void CallBackFunction(void * p_Object, std::string str);
};
#endif //__IAP_MENU_H__