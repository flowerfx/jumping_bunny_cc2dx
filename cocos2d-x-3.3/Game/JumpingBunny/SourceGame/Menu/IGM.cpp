#include "MainMenu.h"
#include "IGM.h"
#include "HUDMenu.h"
#include "GamePlay/ManageObjectAP.h"
#include "Utility/SoundManager.h"
#include "Utility/FileManager.h"
using namespace cocos2d;

IGMenu * IGMenu::m_Instance = NULL;

IGMenu::IGMenu()
{
	p_MenuToSwitchTo = MENU_NONE;

	p_IdxMenuData = 4;
}


IGMenu::~IGMenu()
{
}

void IGMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void IGMenu::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
			MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
			if (p_MenuToSwitchTo == MAINMENU)
			{
				MenuMgr->IsInActionPhase(false);
				ManageObjectAP::GetInstance()->ReleaseTheGame();
			}
			//reset object posion to make sure the menu not stuck
			ResetPositionResource();
		}
		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			OnProcessButton("ContinueBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}
void IGMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer, transform, flags, spriteBatch);
}

void IGMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}

void IGMenu::OnProcess(std::string name)
{
	if (name == "ReloadBtn")
	{
		OnHide();
		p_MenuToSwitchTo = (MENULAYER::ACTION_PHASE);
		HUDMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IGM);
		ManageObjectAP::GetInstance()->ReloadTheGame();
#if USE_CC_AUDIO 
		GetSound->ResumeTheCurrentMusic();
#endif
	}
	else if (name == "MenuBtn")
	{
		OnHide();
		HUDMenu::GetInstance()->OnFadeOut();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
		MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IGM);
#if USE_CC_AUDIO 
		GetSound->ResumeTheCurrentMusic();
#endif
	}
}

void IGMenu::OnProcessButton(std::string name)
{
	if (name == "ReloadBtn")
	{
		MenuMgr->OnShowPopUp("ARE YOU SURE TO RELOAD THE GAME !", 2, name,(void*)this, IGMenu::CallBackFunction);
	}
	else if (name == "ContinueBtn")
	{
		OnHide();
		p_MenuToSwitchTo = (MENULAYER::ACTION_PHASE);
		HUDMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IGM);
#if USE_CC_AUDIO 
		GetSound->ResumeTheCurrentMusic();
#endif
	}
	else if (name == "MenuBtn")
	{
		MenuMgr->OnShowPopUp("ARE YOU SURE TO QUIT THE GAME !", 2, name, (void*)this, IGMenu::CallBackFunction);
	}
	else if (name == "SoundBtn")
	{
		bool Sound = !GetSound->IsEnableSound();

		if (Sound)
		{
			//nothing here
			p_ListSpecificBG.find("mute_sound")->second->Visible(false);
			//
#if USE_CC_AUDIO 
			GetSound->ResetVolume();
#endif
		}
		else
		{
			GetSound->MuteVolume();
			p_ListSpecificBG.find("mute_sound")->second->Visible(true);
		}
		GetSound->SetEnableSound(Sound);
		SaveMgr->SetSoundEnable(Sound ? "1" : "0");
		SaveMgr->SaveTheGameSave();
	}
}

void IGMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void IGMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void IGMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::IGM);
	OnFadeIn();
	//
	bool Sound = GetSound->IsEnableSound();
	if (Sound)
	{
		p_ListSpecificBG.find("mute_sound")->second->Visible(false);
	}
	else
	{
		p_ListSpecificBG.find("mute_sound")->second->Visible(true);
	}
	//
}
void IGMenu::OnHide()
{
	OnFadeOut();
}

void IGMenu::CallBackFunction(void * p_Object, std::string str)
{
	IGMenu * self = (IGMenu*)p_Object;
	self->OnProcess(str);
}