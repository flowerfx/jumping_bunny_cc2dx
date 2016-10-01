#include "OptionsMenu.h"
#include "MainMenu.h"
#include "InfoMenu.h"
using namespace cocos2d;

InfoMenu * InfoMenu::m_Instance = NULL;

InfoMenu::InfoMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IdxMenuData = 2;
}


InfoMenu::~InfoMenu()
{
}

void InfoMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	p_DisplayString.find("Text31")->second->SetStringName("1.0.1.1");
}


void InfoMenu::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
			MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
			//reset object posion to make sure the menu not stuck
			ResetPositionResource();
		}
		else if (p_curStatus == STATUS_MENU::M_FADEIN)
		{
			MainMenu::GetInstance()->OnSetVisibleBtn("*", false);
		}
		p_curStatus = STATUS_MENU::M_IDLE;

		if (IsBackKeyPress())
		{
			OnProcessButton("OKBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}
void InfoMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer, transform, flags,  spriteBatch );
}
void InfoMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}

void InfoMenu::OnProcessButton(std::string name)
{
	if (name == "OKBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
		MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::INFOMENU);
	}
}

void InfoMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void InfoMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void InfoMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::INFOMENU);
	OnFadeIn();
}
void InfoMenu::OnHide()
{
	OnFadeOut();
}