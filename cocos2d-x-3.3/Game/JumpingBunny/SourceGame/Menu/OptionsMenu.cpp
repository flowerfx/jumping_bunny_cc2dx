#include "OptionsMenu.h"
#include "MainMenu.h"
#include "Social/SocialManager.h"
#include "Utility/SoundManager.h"
#include "Utility/FileManager.h"
#include "FXFlatform/XFlatform.h"
using namespace cocos2d;

OptionsMenu * OptionsMenu::m_Instance = NULL;

OptionsMenu::OptionsMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	idxFbBtn = -1;
	p_IdxMenuData = 1;
}


OptionsMenu::~OptionsMenu()
{
}

void OptionsMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void OptionsMenu::Update(float dt)
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

		}
		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			OnProcessButton("BackBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime , dt * 100);
	}
	//
	SetColorFBBtn();
	//
	if (FXSocialService::SocialMgr->GetFacebookProfile()->AccessToken() == "")
	{
		p_ListSpecificBG.find("online_ring_fb")->second->Visible(false);
		p_ListSpecificBG.find("offline_ring_fb")->second->Visible(true);
	}
	else
	{
		p_ListSpecificBG.find("online_ring_fb")->second->Visible(true);
		p_ListSpecificBG.find("offline_ring_fb")->second->Visible(false);
	}
}
void OptionsMenu::SetColorFBBtn()
{
	if (FXSocialService::SocialMgr->IsDownloading())
	{
		if (idxFbBtn < 0)
		{
			for (int i = 0; i < p_ListButton.size(); i++)
			{
				if (p_ListButton.at(i)->GetName() == "fb_btn")
				{
					idxFbBtn = i;
				}
			}
		}
		else
		{
			p_ListButton.at(idxFbBtn)->GetResource()->setColor(Color3B::GRAY);
		}
	}
	else
	{
		if (idxFbBtn < 0)
		{
			for (int i = 0; i < p_ListButton.size(); i++)
			{
				if (p_ListButton.at(i)->GetName() == "fb_btn")
				{
					idxFbBtn = i;
				}
			}
		}
		else
		{
			p_ListButton.at(idxFbBtn)->GetResource()->setColor(Color3B::WHITE);
		}
	}
}
void OptionsMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit( renderer,  transform,  flags,   spriteBatch );
}
void OptionsMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags,spriteBatch);
}

void OptionsMenu::OnProcessButton(std::string name)
{
	if (name == "SoundBtn")
	{
		bool Sound = !GetSound->IsEnableSound();
		GetSound->SetEnableSound(Sound);

		if (Sound)
		{
			//nothing here
			p_ListSpecificBG.find("mute_sound")->second->Visible(false);
			//
			GetSound->ResetVolume();
		}
		else
		{
			GetSound->MuteVolume();
			p_ListSpecificBG.find("mute_sound")->second->Visible(true);
		}
		SaveMgr->SetSoundEnable(Sound ? "1" : "0");
		SaveMgr->SaveTheGameSave();
	}
	else if (name == "BackBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
		MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::OPTIONMENU);
	}
	else if (name == "InfoBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::INFOMENU;
	}
	else if (name == "RateBtn")
	{
		XFlatform::OnRateTheGame();
	}
	else if (name == "fb_btn")
	{
		if (FXSocialService::SocialMgr->IsDownloading())
		{
			return;
		}
		//
		if (!XFlatform::IsHaveInternetConnection())
		{
			MenuMgr->OnShowPopUp("\nNO INTERNET CONNECTION !\n TRY AGAIN LATER", 1);
		}
		//
		if (FXSocialService::SocialMgr->GetFacebookProfile()->AccessToken() == "")
		{
			XFlatform::FB_OpenLoginPage();
		}
	}
}

void OptionsMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void OptionsMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void OptionsMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::OPTIONMENU);
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
void OptionsMenu::OnHide()
{
	OnFadeOut();
}