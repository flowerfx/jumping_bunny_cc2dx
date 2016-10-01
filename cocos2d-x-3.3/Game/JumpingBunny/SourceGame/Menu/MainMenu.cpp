#include "MainMenu.h"
#include "Social/SocialManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "FXFlatform/XFlatform.h"
using namespace cocos2d;

MainMenu * MainMenu::m_Instance = NULL;

MainMenu::MainMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_MenuReturnTo = MENU_NONE;
	p_IsExitGame = false;

	p_IdxMenuData = 0;
    
    p_Avatar = NULL;
}


MainMenu::~MainMenu()
{
    p_Avatar = NULL;
}

void MainMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void MainMenu::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			MenuMgr->OnShowPopUp("QUIT THE GAME ?", 2, "", (void*)this, MainMenu::CallBackFunction);
			SetBackkey(false);
		}
		SetCoin();
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}
void MainMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisit( renderer,   transform,  flags,  spriteBatch );
	if (p_Avatar)
	{
		p_Avatar->visit();
	}
}
void MainMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
	if (p_Avatar)
	{
		p_Avatar->draw(renderer, transform, flags);
	}
}

void MainMenu::OnProcessButton(std::string name)
{
	if (name == "PlayBtn")
	{
		MenuMgr->SwitchToMenu(MENULAYER::GAMEPLAYSELECTMENU);
	}
	else if (name == "OptionBtn")
	{
		MenuMgr->SwitchToMenu(MENULAYER::OPTIONMENU);
		OnSetVisibleBtn("OptionBtn", false);
	}
	else if (name == "ShopBtn")
	{
		if (!XFlatform::IsHaveInternetConnection())
		{
			MenuMgr->OnShowPopUp("NO INTERNET CONNECTION!\nTRY AGAIN LATER !", 1);
			return;
		}
		MenuMgr->SwitchToMenu(MENULAYER::IAP);
	}
	else if (name == "BoostBtn")
	{
		MenuMgr->SwitchToMenu(MENULAYER::BOOSTMENU);
	}
	else if (name == "leaderboardBtn")
	{
		MenuMgr->SwitchToMenu(MENULAYER::LEADERBOARD);
	}
}

void MainMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void MainMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void MainMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::MAINMENU);
	if (p_MenuReturnTo == OPTIONMENU)
	{
		OnSetVisibleBtn("OptionBtn", true);
	}
	else if (p_MenuReturnTo == INFOMENU 
		|| p_MenuReturnTo == IGM 
		|| p_MenuReturnTo == RESULTMENU
		|| p_MenuReturnTo == IAP
		|| p_MenuReturnTo == GAMEPLAYSELECTMENU)
	{
		OnSetVisibleBtn("*", true);
	}
	p_MenuReturnTo = MENU_NONE;
	OnFadeIn();
}
void MainMenu::OnHide()
{
	OnFadeOut();
}

void MainMenu::OnSetVisibleBtn(std::string name_btn, bool IsVisible)
{
	for (int i = 0; i < p_ListButton.size(); i++)
	{
		if ((p_ListButton.at(i)->GetName() == name_btn) || name_btn == "*")
		{
			p_ListButton.at(i)->Visible(IsVisible);
		}
	}
}

void MainMenu::CallBackFunction(void * p_Object, std::string str)
{
	MainMenu * self = (MainMenu*)p_Object;
	self->callExitGame();
}
void MainMenu::callExitGame()
{
#ifdef WP8
	extern void ExitApp();
	ExitApp();
#endif
}

void MainMenu::SetCoin()
{
	std::string str = std::to_string(MenuMgr->Getcoin());
	if (p_DisplayString.size() <= 0)
		return;
	p_DisplayString.at("Text0")->SetStringName(str);
}

void MainMenu::DisplayProfileFB(std::wstring name, std::string imgPath)
{
	if (p_DisplayString.size() <= 0)
		return;
	p_DisplayString.at("Text2")->SetStringName(std::string(CCUnicodeToUtf8(name.data())));
	//
	if (imgPath == "")
		return;
	if (!p_Avatar)
	{
		p_Avatar = Sprite::create(imgPath);
		p_Avatar->retain();
	}
	else
	{
		p_Avatar = Sprite::create(imgPath);
	}
	auto winsize = GetWinSize();
	p_Avatar->setPosition(60.f / 800.f * winsize.width, (480.f - 52.f) / 480.f * winsize.height);
	Point p_scale(1, 1);
	p_scale.x = (80.f * (winsize.width  / 800.f)) / p_Avatar->getContentSize().width;
	p_scale.y = (80.f * (winsize.height / 480.f)) / p_Avatar->getContentSize().height;
	p_Avatar->setScaleX(p_scale.x);
	p_Avatar->setScaleY(p_scale.y);
}