#include "MainMenu.h"
#include "HUDMenu.h"
#include "ResultMenu.h"
#include "GamePlay/ManageObjectAP.h"
#include "Social/SocialManager.h"
#include "FXFlatform/XFlatform.h"
#include "Particle/ParicleManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
#include "GamePlay/APManager.h"
#include "LevelSelectionMenu.h"
using namespace cocos2d;

ResultMenu * ResultMenu::m_Instance = NULL;

ResultMenu::ResultMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IsGainHScore = false;
	p_IdxMenuData = 5;

	p_OriginScaleTitle = Vec2(0,0);
	p_DeltaScale = Vec2(0.2,0.1);
	p_ScaleDistance = Vec2(0, 0);
	IsFinishFadeIn = false;

	IsStartShowStar = false;

	p_IsContinueGameLevel = false;
	p_IsThreadFinish = true; 
}


ResultMenu::~ResultMenu()
{
	p_IsThreadFinish = true;
}

void ResultMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	//
	p_OriginScaleTitle = p_DisplayString.find("Text4")->second->GetScale();
	//
	p_timeStar[0].OriginScaleStar = GetBackgroundByName("w_star_1")->GetScale();
	p_timeStar[1].OriginScaleStar = GetBackgroundByName("w_zstar_2")->GetScale();
	p_timeStar[2].OriginScaleStar = GetBackgroundByName("w_star_3")->GetScale();
}

void ResultMenu::SetScoreGame(TYPE_END_RESULT result)
{
	//
	int score = ManageObjectAP::GetInstance()->GetScore();
	std::string str = std::to_string(score);
	p_DisplayString.at("Text2")->SetStringName(str);
	int HScore = MenuMgr->GetHScore();
	//
	std::string nameGP = APMgr->GetCurrentNameLevel();
	auto cur_gameplay = XMLMgr->GetGamePlayMgr()->GetGamePlay(nameGP);
	float originTime = cur_gameplay->GetTimeCoutDown();
	float currentcdTime = OBJAPMgr->GetCurrentTimeCD();
	float finalTime = (originTime - currentcdTime) >= 0 ? (originTime - currentcdTime) : 0;
	//
	int p_CurrentLevelIdx = APMgr->GetCurrentPartPlay();
	//
	int maxLevReach = SaveMgr->GetMaxLevelReach();

	if (result == TYPE_END_RESULT::RESULT_PASS_H_SCORE)
	{
		MenuMgr->SetHScore(score);
		//push the highest score to server FB
		FXSocialService::SocialMgr->GetLeaderB()->UpdateHighScoreUserToLB(score);
		//
		SaveMgr->SaveTheGameSave();
		HScore = score;
		p_IsGainHScore = true;
		p_DisplayString.find("Text4")->second->SetStringName("YOU GAIN HIGH SCORE!");
	}
	else if (result == TYPE_END_RESULT::RESULT_DIE)
	{
		p_IsGainHScore = false;
		p_DisplayString.find("Text4")->second->SetStringName("OOP! YOU DIE");
	}
	else if (result == TYPE_END_RESULT::RESULT_END_TIME)
	{
		p_IsGainHScore = false;
		p_DisplayString.find("Text4")->second->SetStringName("OOP! TIME OUT");
		GetButtonByName("MenuBtn")->Visible(false);
	}
	else if (result == TYPE_END_RESULT::RESULT_PASS_LEVEL)
	{
		p_IsGainHScore = true;
		//check if is this a newest level;
		if (p_CurrentLevelIdx >= maxLevReach)
		{
			//save the current level
			bool res = SaveMgr->InsertLevelGain(nameGP, p_CurrentLevelIdx, finalTime, OBJAPMgr->GetCurrentStarGain(),false);
			//set detail at level
			res = MenuMgr->OnSetDetailAtLevelIdx(p_CurrentLevelIdx, OBJAPMgr->GetCurrentStarGain(), finalTime);
			//
			if (res && p_CurrentLevelIdx < NUMBER_LEVEL)
			{
				int res = MenuMgr->OnUnlockLevelIdx(p_CurrentLevelIdx + 1);
				if (res == 0)
				{
					//unlock next level
					SaveMgr->InsertLevelGain(APMgr->convertToPartName(p_CurrentLevelIdx + 1), p_CurrentLevelIdx + 1, 0, 0);
				}

			}
			//save to data
			SaveMgr->SaveTheLevelFromFile();
			//
			p_DisplayString.find("Text4")->second->SetStringName(std::string("YOU PASS LEVEL ") + std::to_string(p_CurrentLevelIdx + 1));
		}
		else if (p_CurrentLevelIdx < maxLevReach)
		{
			bool res = MenuMgr->OnSetDetailAtLevelIdx(p_CurrentLevelIdx, OBJAPMgr->GetCurrentStarGain(), finalTime);
			//
			if (res)
			{
				//modified in save mgr
				auto it = SaveMgr->GetLevelWithName(nameGP);
				//
				if (it->star_gain <= OBJAPMgr->GetCurrentStarGain()) 
				{ 
					it->star_gain = OBJAPMgr->GetCurrentStarGain(); 
					if (it->timePlay < finalTime) { 
						it->timePlay = finalTime; 
					}
				}
				//
				if (p_CurrentLevelIdx == maxLevReach - 1)
				{
					//unlock next level
					int res = MenuMgr->OnUnlockLevelIdx(p_CurrentLevelIdx + 1);
					if (res == 0)
					{
						//unlock next level save game
						SaveMgr->InsertLevelGain(APMgr->convertToPartName(p_CurrentLevelIdx + 1), p_CurrentLevelIdx + 1, 0, 0);
					}
					//
					p_DisplayString.find("Text4")->second->SetStringName(std::string("YOU PASS LEVEL ") + std::to_string(p_CurrentLevelIdx + 1));
				}
				else
				{
					//
					if (p_CurrentLevelIdx == ((p_CurrentLevelIdx / NUMBER_LEVEL_PER_SESSION) + 1)*NUMBER_LEVEL_PER_SESSION - 1)
					{
						MenuMgr->OnUnlockLevelIdx(p_CurrentLevelIdx + 1);
					}
					//
					p_DisplayString.find("Text4")->second->SetStringName(std::string("YOU GAIN H-SCORE L") + std::to_string(p_CurrentLevelIdx + 1));
				}
				// save
				SaveMgr->SaveTheLevelFromFile();
			}
			else
			{
				// no thing happend
				p_DisplayString.find("Text4")->second->SetStringName(std::string("OOP! NO NEW RECORD"));
			}
		}
	}

	if (APMgr->IsOnStoryMode())
	{
		p_DisplayString.at("Text0")->IsVisible(false);
		p_DisplayString.at("Text1")->IsVisible(false);
		p_DisplayString.at("Text2")->IsVisible(false);
		p_DisplayString.at("Text3")->IsVisible(false);
		//show the star 
		GetBackgroundByName("w_star_1")->Visible(false);
		GetBackgroundByName("t_star_black_1")->Visible(true);
		GetBackgroundByName("w_zstar_2")->Visible(false);
		GetBackgroundByName("t_zstar_black_2")->Visible(true);
		GetBackgroundByName("w_star_3")->Visible(false);
		GetBackgroundByName("t_star_black_3")->Visible(true);
		//
		GetBackgroundByName("time_panel")->Visible(true);
		p_DisplayString.at("Text5")->IsVisible(true);
		p_DisplayString.at("Text6")->IsVisible(true);
		//
		p_DisplayString.at("Text6")->SetStringName(convertToDisplayTime(finalTime));
		//
		GetButtonByName("MenuBtn")->Visible(false);
		GetButtonByName("CloseBtn")->Visible(true);
		//if time out at the top level gameplay, do not play next mission
		GetButtonByName("ContinueBtn")->Visible(p_CurrentLevelIdx < maxLevReach);
	}
	else
	{
		p_DisplayString.at("Text0")->IsVisible(true);
		p_DisplayString.at("Text1")->IsVisible(true);
		p_DisplayString.at("Text2")->IsVisible(true);
		p_DisplayString.at("Text3")->IsVisible(true);
		//hide the star
		GetBackgroundByName("w_star_1")->Visible(false);
		GetBackgroundByName("t_star_black_1")->Visible(false);
		GetBackgroundByName("w_zstar_2")->Visible(false);
		GetBackgroundByName("t_zstar_black_2")->Visible(false);
		GetBackgroundByName("w_star_3")->Visible(false);
		GetBackgroundByName("t_star_black_3")->Visible(false);
		//
		GetBackgroundByName("time_panel")->Visible(false);
		p_DisplayString.at("Text5")->IsVisible(false);
		p_DisplayString.at("Text6")->IsVisible(false);
		//
		GetButtonByName("MenuBtn")->Visible(true);
		GetButtonByName("CloseBtn")->Visible(false);
		GetButtonByName("ContinueBtn")->Visible(false);
	}

	if (result != TYPE_END_RESULT::RESULT_PASS_LEVEL)
	{
		//check and update score and push to server App42
		FXSocialService::SocialMgr->GetLeaderB()->UpdateScore(score);
		//
		str = std::to_string(HScore);
		p_DisplayString.at("Text3")->SetStringName(str);
		//zoom and fade in effect
		//p_DisplayString.find("Text4")->second->se
	}
}
void ResultMenu::Update(float dt)
{
	if (XFlatform::FB_IsSharePanelAppear())
	{
		if (IsBackKeyPress())
		{
			XFlatform::FB_CloseSharePanel();
			SetBackkey(false);
		}
		return;
	}
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
			if (p_MenuToSwitchTo == MAINMENU)
			{
				MenuMgr->IsInActionPhase(false);
				ManageObjectAP::GetInstance()->ReleaseTheGame(true);
				MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
				//reset object posion to make sure the menu not stuck
				ResetPositionResource();
			}
			else if (p_MenuToSwitchTo == ACTION_PHASE)
			{
				if (p_IsContinueGameLevel)
				{
#if USE_CREATE_GAMEPLAY_IN_THREAD
					if (p_IsThreadFinish)
					{
#endif
						MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
						//reset object posion to make sure the menu not stuck
						ResetPositionResource();
						p_IsContinueGameLevel = false;
						MenuMgr->IsInActionPhase(true);
#if USE_CREATE_GAMEPLAY_IN_THREAD
						if (p_ThreadCreateGame.joinable())
						{
							p_ThreadCreateGame.join();
						}
#endif
					}
#if USE_CREATE_GAMEPLAY_IN_THREAD
					else
					{
						return;
					}
#endif
				}
				else
				{
					MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
					//reset object posion to make sure the menu not stuck
					ResetPositionResource();
				}
			}
		}
		else if (p_curStatus == STATUS_MENU::M_FADEIN)
		{
			IsFinishFadeIn = true;
			p_DisplayString.find("Text4")->second->IsVisible(true);
			if (p_IsGainHScore)
			{
#if USE_CC_AUDIO
				GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_GAIN_HSCORE"));
#endif
				//play particle
				EffectMgr->AddParticle(PARTICLE_TYPE::P_WIN_EFFECT, Point(GetWinSize().width / 2, GetWinSize().height + 10), "Win_Global", true);
			}
			else
			{
#if USE_CC_AUDIO
				GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_GAME_OVER"));
#endif
			}

		}
		if (IsFinishFadeIn)
		{
			p_ScaleDistance -= p_DeltaScale;
			if (p_ScaleDistance.x <= p_OriginScaleTitle.x)
			{
				IsFinishFadeIn = false;
				if (APMgr->IsOnStoryMode())
				{
					if (OBJAPMgr->GetCurrentStarGain() > 0)
					{
						IsStartShowStar = true;
						p_timeStar[0].OnShow = true;
						GetBackgroundByName("w_star_1")->Visible(p_timeStar[0].OnShow);
					}
				}
				p_ScaleDistance = p_OriginScaleTitle;
			}
			p_DisplayString.find("Text4")->second->SetScale(p_ScaleDistance);
		}

		if (IsStartShowStar)
		{
			for (int idx = 0; idx < 3; idx++)
			{
				if (p_timeStar[idx].OnShow)
				{
					p_timeStar[idx].ScaleDistanceStar -= Vec2(p_timeStar[idx].DeltaScaleStar, p_timeStar[idx].DeltaScaleStar);
					if (p_timeStar[idx].ScaleDistanceStar.x <= p_timeStar[idx].OriginScaleStar.x)
					{
						p_timeStar[idx].OnShow = false;
						if (idx < 3 - 1)
						{
							if (OBJAPMgr->GetCurrentStarGain() > idx + 1)
							{
								p_timeStar[idx + 1].OnShow = true;
							}
							else
							{
								IsStartShowStar = false;
							}
						}
						else
						{
							IsStartShowStar = false;
						}
						p_timeStar[idx].ScaleDistanceStar = p_timeStar[idx].OriginScaleStar;
					}
					if (idx == 0)
					{
						GetBackgroundByName("w_star_1")->SetRealScale(p_timeStar[idx].ScaleDistanceStar);
						GetBackgroundByName("w_star_1")->Visible(true);
					}
					else if (idx == 1)
					{
						GetBackgroundByName("w_zstar_2")->SetRealScale(p_timeStar[idx].ScaleDistanceStar);
						GetBackgroundByName("w_zstar_2")->Visible(true);
					}
					else if (idx == 2)
					{
						GetBackgroundByName("w_star_3")->SetRealScale(p_timeStar[idx].ScaleDistanceStar);
						GetBackgroundByName("w_star_3")->Visible(true);
					}
				}
			}			

		}


		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			OnProcessButton("ReloadBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}
void ResultMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer, transform, flags, spriteBatch);
}
void ResultMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}
void ResultMenu::Process(std::string name)
{
	if (name == "MenuBtn" || name == "CloseBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
		MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::RESULTMENU);
		//
#if USE_CC_AUDIO
		GetSound->PlayCurrentMusic();
#endif
	}
	else if (name == "fb_btn")
	{
		XFlatform::FB_OpenLoginPage();
	}
}
void ResultMenu::OnProcessButton(std::string name)
{
	if (name == "ReloadBtn")
	{
		OnHide();
		p_MenuToSwitchTo = (MENULAYER::ACTION_PHASE);
		ManageObjectAP::GetInstance()->ReloadTheGame();
#if USE_CC_AUDIO
		GetSound->PlayCurrentMusic();
#endif
	}
	else if (name == "MenuBtn" || name == "CloseBtn")
	{
		MenuMgr->OnShowPopUp("ARE YOU SURE WANT TO QUIT ?", 2, name, (void*)this, ResultMenu::CallBackFunction);
	}
	else if (name == "fb_btn")
	{
		if (!XFlatform::IsHaveInternetConnection())
		{
			MenuMgr->OnShowPopUp("NO INTERNET CONNECTION!\nTRY AGAIN LATER", 1);
			return;
		}
		if (!FXSocialService::SocialMgr->GetFacebookProfile()->IsGetProfileSuceed())
		{
			MenuMgr->OnShowPopUp("YOU NOT LOGIN TO FACEBOOK! \nLOGIN & SHARE NOW?", 2, name, (void*)this, ResultMenu::CallBackFunction);
			return;
		}
		string str_share = "";
		//std::string text = "this is test string!";
		if (!APMgr->IsOnStoryMode())
		{
			str_share = "i just gain " + std::to_string(ManageObjectAP::GetInstance()->GetScore()) + " score in jumping bunny!";
		}
		else
		{
			str_share = "i've just reached " + std::to_string(OBJAPMgr->GetCurrentStarGain()) + " stars in level " + std::to_string(APMgr->GetCurrentPartPlay()) + " of jumping bunny";
		}
			//
		FXSocialService::SocialMgr->ShareOnFB(str_share);
	}
	else if (name == "ContinueBtn")
	{
		//to do :
		if (!MenuMgr->IsThisSessionUnlock((APMgr->GetCurrentPartPlay() + 1) / NUMBER_LEVEL_PER_SESSION))
		{
			MenuMgr->OnShowPopUp("You dont have enought star \n to continue next level!", 1);
			return;
		}
		OnHide();
		p_MenuToSwitchTo = (MENULAYER::ACTION_PHASE);
		p_IsContinueGameLevel = true;
		int idxLevel = APMgr->GetCurrentPartPlay();
		APMgr->SetCurrentPartPlay(idxLevel + 1);
		ManageObjectAP::GetInstance()->ReleaseTheGame();
#if USE_CREATE_GAMEPLAY_IN_THREAD
		p_IsThreadFinish = false;
		MenuMgr->IsInActionPhase(false);
		p_ThreadCreateGame = std::thread(&ResultMenu::OnCreateNextLevel, this);
#else
		p_IsThreadFinish = true;
		OnCreateNextLevel();
#endif

#if USE_CC_AUDIO
		GetSound->PlayCurrentMusic();
#endif
	}
}

void ResultMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void ResultMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void ResultMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::RESULTMENU);
	//
	OnFadeIn();
	//
	IAPMgr->ShowAds();
	//
	IsFinishFadeIn = false;
	//
	p_ScaleDistance = Vec2(4.f, 2.f);
	p_DisplayString.find("Text4")->second->IsVisible(false);
	p_DisplayString.find("Text4")->second->SetScale(p_ScaleDistance);
	//
	if (APMgr->IsOnStoryMode())
	{
		//check star //
		if (OBJAPMgr->GetCurrentStarGain() > 0)
		{
			p_timeStar[0].ScaleDistanceStar = p_timeStar[0].OriginScaleStar * p_timeStar[0].DistanceScaleStar;
			GetBackgroundByName("w_star_1")->SetScale(p_timeStar[0].ScaleDistanceStar);
		}
		//
		if (OBJAPMgr->GetCurrentStarGain() > 1)
		{
			p_timeStar[1].ScaleDistanceStar = p_timeStar[1].OriginScaleStar * p_timeStar[1].DistanceScaleStar;
			GetBackgroundByName("w_zstar_2")->SetScale(p_timeStar[1].ScaleDistanceStar);
		}
		//
		if (OBJAPMgr->GetCurrentStarGain() > 2)
		{
			p_timeStar[2].ScaleDistanceStar = p_timeStar[2].OriginScaleStar * p_timeStar[2].DistanceScaleStar;
			GetBackgroundByName("w_star_3")->SetScale(p_timeStar[2].ScaleDistanceStar);
		}
	}
}
void ResultMenu::OnHide()
{
	OnFadeOut();
	//
	EffectMgr->KillParticle("Win_Global");
}
void ResultMenu::CallBackFunction(void * p_Object, std::string str)
{
	ResultMenu * self = (ResultMenu*)p_Object;
	self->Process(str);
}

void ResultMenu::OnCreateNextLevel()
{
	p_IsThreadFinish = false;
	APMgr->InitWithGamePlay(APMgr->GetCurrentGamePlay());
	p_IsThreadFinish = true;
}