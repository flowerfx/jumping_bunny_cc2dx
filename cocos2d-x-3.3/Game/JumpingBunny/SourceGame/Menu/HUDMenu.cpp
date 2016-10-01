#include "OptionsMenu.h"
#include "MainMenu.h"
#include "HUDMenu.h"
#include "GamePlay/ManageObjectAP.h"
#include "Utility/BoostManager.h"
#include "GamePlay/APManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "Social/SocialManager.h"



using namespace cocos2d;
using namespace FXSocialService;
HUDMenu * HUDMenu::m_Instance = NULL;

HUDMenu::HUDMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IdxMenuData = 3;

	//
	p_OriginScaleTimeCD = p_ScaleDistanceTimeCD = Vec2(0,0);
	p_DeltaScaleTimeCD = 0.05;
	p_ScaleMaxTimeCD = 2.f;
	p_DirectScaleTimeCD = 1;
}


HUDMenu::~HUDMenu()
{
}

void HUDMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	//
	p_OriginScaleTimeCD = p_DisplayString.find("Text_count_down")->second->GetScale();
}


void HUDMenu::Update(float dt)
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
		p_MenuToSwitchTo = MENU_NONE;
		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			OnProcessButton("PauseBtn");
			SetBackkey(false);
		}
		//check score on endless mode and runner mode
		OnCheckScoreWithScoreBoard();
		//check the condition
		if (APMgr->GetCurrentGamePlay() != TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN)
		{
			//get string text cd
			auto str_text_cd = p_DisplayString.find("Text_count_down")->second;
			//show time count down
			float current_time_cd = OBJAPMgr->GetCurrentTimeCD();
			str_text_cd->SetStringName(convertToDisplayTime(current_time_cd));
			//
			if (current_time_cd <= 5.f)
			{
				p_OnDelayTime--;
				if (p_OnDelayTime <= 0)
				{
					p_OnDelayTime = 0;
					p_ScaleDistanceTimeCD -= p_DirectScaleTimeCD * Vec2(p_DeltaScaleTimeCD, p_DeltaScaleTimeCD);
					if (p_ScaleDistanceTimeCD < p_OriginScaleTimeCD)
					{
						p_ScaleDistanceTimeCD = p_OriginScaleTimeCD;
						p_DirectScaleTimeCD *= -1;
						p_OnDelayTime = 10.f;
					}
					else if (p_ScaleDistanceTimeCD > (p_OriginScaleTimeCD * p_ScaleMaxTimeCD))
					{
						p_ScaleDistanceTimeCD = p_OriginScaleTimeCD * p_ScaleMaxTimeCD;
						p_DirectScaleTimeCD *= -1;
					}
				}
				str_text_cd->SetScale(p_ScaleDistanceTimeCD);
			}
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
	std::string str = std::to_string(ManageObjectAP::GetInstance()->GetScore());
	p_DisplayString.at("Text0")->SetStringName(str);
	//

	for (int i = 1; i <= TYPE_BOOST::B_LIFE_BOOST; i++)
	{
		if (!APMgr->IsUseTheBoost((TYPE_BOOST)i))
		{
			p_ListBackground.at(i)->GetResource()->setColor(Color3B(100, 100, 100));
		}
		else
		{
			p_ListBackground.at(i)->GetResource()->setColor(Color3B(255, 255, 255));
		}
	}
}
void HUDMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisit(renderer, transform,  flags,  spriteBatch );
}
void HUDMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}

void HUDMenu::OnProcessButton(std::string name)
{
	if (name == "PauseBtn")
	{
		//OnHide();
		MenuMgr->SwitchToMenu( MENULAYER::IGM);
		//pause the music
#if USE_CC_AUDIO 
		if (GetSound->IsMusicPlaying())
		{
			GetSound->PauseTheCurrentMusic();
		}
#endif	
	}
	else if (name == "BoostBtn")
	{
#ifdef USE_CHEAT_BTN
		OBJAPMgr->OnPassLevel();
#else
		MenuMgr->SwitchToMenu(MENULAYER::BOOSTMENU);
#endif
	}
}

void HUDMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void HUDMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void HUDMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::ACTION_PHASE);
	MenuMgr->IsInActionPhase(true);
	if (p_MenuReturnTo == IGM || p_MenuReturnTo == BOOSTMENU)
	{
		p_MenuReturnTo = MENU_NONE;
		return;
	}
	//
	OnCabrivableNumberBoostItem();
	//
	OnFadeIn();
	//
	bool ShowBotPanel = true;
	if (APMgr->IsOnStoryMode())
	{
		ShowBotPanel = false;
	}

	p_DisplayString.find("Text3")->second->IsVisible(ShowBotPanel);
	p_DisplayString.find("Text4")->second->IsVisible(ShowBotPanel);
	p_DisplayString.find("Text5")->second->IsVisible(ShowBotPanel);
	GetBackgroundByName("panel_board")->Visible(ShowBotPanel);
	p_DisplayString.find("Text0")->second->IsVisible(ShowBotPanel);
	p_DisplayString.find("Text1")->second->IsVisible(ShowBotPanel);
	//
	GetBackgroundByName("icon_gravity")->Visible(!ShowBotPanel);
	p_DisplayString.find("Text_gra_num")->second->IsVisible(!ShowBotPanel);

	GetBackgroundByName("icon_jump")->Visible(ShowBotPanel);
	p_DisplayString.find("Text_jump_num")->second->IsVisible(ShowBotPanel);

	GetBackgroundByName("icon_life")->Visible(ShowBotPanel);
	p_DisplayString.find("Text_life_num")->second->IsVisible(ShowBotPanel);

	if (APMgr->GetCurrentGamePlay() == TYPE_GAMEPLAY::GAMEPLAY_ENDLESS_RUN)
	{
		p_DisplayString.find("Text_count_down")->second->IsVisible(false);
	}
	else
	{
		p_DisplayString.find("Text_count_down")->second->IsVisible(true);
		p_DisplayString.find("Text_count_down")->second->SetScale(p_OriginScaleTimeCD);
	}


}
void HUDMenu::OnHide()
{
	OnFadeOut();
}

//
void HUDMenu::OnCabrivableNumberBoostItem()
{
	std::string str = std::to_string(BoostMgr->GetItemData(ListItemName[0]));
	p_DisplayString.at("Text_gra_num")->SetStringName(str);
	str = std::to_string(BoostMgr->GetItemData(ListItemName[1]));
	p_DisplayString.at("Text_jump_num")->SetStringName(str);
	str = std::to_string(BoostMgr->GetItemData(ListItemName[2]));
	p_DisplayString.at("Text_life_num")->SetStringName(str);
}

//
void HUDMenu::OnCheckScoreWithScoreBoard()
{
	if (APMgr->IsOnStoryMode())
	{
		return;
	}
	int cur_score = ManageObjectAP::GetInstance()->GetScore();
	int idxScoreBoardReach = -1;
	auto listScore = SocialMgr->GetLeaderB()->GetTopPlayerScores();
	if (listScore.size() > 0)
	{
		for (int i = (int)listScore.size() - 1; i >= 0; i--)
		{
			if (listScore.at(i)->Score > cur_score)
			{
				idxScoreBoardReach = i;
				break;
			}
		}
	}
	std::string str_score = "";
	std::string str_rank = "";
	size_t score_need_to_reach = cur_score;
	//
	if (idxScoreBoardReach > -1)
	{
		score_need_to_reach = listScore.at(idxScoreBoardReach)->Score;
		str_score = std::to_string(score_need_to_reach);
		str_rank = std::string("#") + std::to_string(idxScoreBoardReach + 1) + std::string(": ");
	}
	else
	{
		str_score = std::to_string(score_need_to_reach);
		str_rank = std::string("#you top") + std::string(": ");
	}
	p_DisplayString.find("Text4")->second->SetStringName(str_rank);
	p_DisplayString.find("Text5")->second->SetStringName(str_score);
	//


}

//
void HUDMenu::ResetScaleCDTimer()
{
	p_DisplayString.find("Text_count_down")->second->SetScale(p_OriginScaleTimeCD);
}