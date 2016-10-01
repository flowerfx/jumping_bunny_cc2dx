#include "Menu/MenuManager.h"
#include "GamePlay/APManager.h"
#include "Menu/MainMenu.h"
#include "Menu/OptionsMenu.h"
#include "Menu/InfoMenu.h"
#include "Menu/HUDMenu.h"
#include "Menu/IGM.h"
#include "Menu/ResultMenu.h"
#include "Menu/IAPMenu.h"
#include "Menu/PopUpMenu.h"
#include "Menu/BoostMenu.h"
#include "Menu/LeaderBoardMenu.h"
#include "Menu/GamePlaySelection.h"
#include "Menu/LevelSelectionMenu.h"
MenuManager * MenuManager::m_Instance = NULL;
MenuManager::MenuManager()
{
	p_CurrentMenuLayer = MAINMENU;
	p_SkyObjectBG = new BGObject();
	p_HScore = 0;
	p_IsInActionPhase = false;
	p_IsPopUpAppear = false;

	p_StateLoadInit = 0;
}
MenuManager::~MenuManager()
{
	CC_SAFE_RELEASE(p_SkyObjectBG);
}
int MenuManager::Init()
{
	if (p_StateLoadInit == 0)
	{
		MainMenu::GetInstance()->Init();
		OptionsMenu::GetInstance()->Init();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	if (p_StateLoadInit == 1)
	{
		InfoMenu::GetInstance()->Init();
		HUDMenu::GetInstance()->Init();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	if (p_StateLoadInit == 2)
	{
		IGMenu::GetInstance()->Init();
		ResultMenu::GetInstance()->Init();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	if (p_StateLoadInit == 3)
	{
		IAPMenu::GetInstance()->Init();
		BoostMenu::GetInstance()->Init();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	if (p_StateLoadInit == 4)
	{
		LeaderBoardMenu::GetInstance()->Init();
		GamePlaySelection::GetInstance()->Init();
		GetPopUp->Init();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	if (p_StateLoadInit == 5)
	{
		std::string sky_type = "sky_bg";
		LevelSelectionMenu::GetInstance()->Init();
#if  USE_SKY_MENU
		sky_type = "sky";
#endif
		p_SkyObjectBG->InitTheValue(sky_type);
		//init the first value
		p_HScore = SaveMgr->GetDataSave()->GetHScore();
		p_Coin = SaveMgr->GetDataSave()->GetCoin();
		p_StateLoadInit++;
		return p_StateLoadInit;
	}
	return -1;
}
void MenuManager::Update(float dt)
{
	switch (p_CurrentMenuLayer)
	{
	case MAINMENU:
		MainMenu::GetInstance()->Update(dt);
		break;
	case OPTIONMENU:
		OptionsMenu::GetInstance()->Update(dt);
		break;
	case INFOMENU:
		InfoMenu::GetInstance()->Update(dt);
		break;
	case LOADMENU:
		break;
	case ACTION_PHASE:
		HUDMenu::GetInstance()->Update(dt);
		break;
	case IGM:
		if (IGMenu::GetInstance()->GetMenuSwithTo() == MAINMENU)
		{
			HUDMenu::GetInstance()->Update(dt);
		}
		IGMenu::GetInstance()->Update(dt);
		break;
	case RESULTMENU:
		ResultMenu::GetInstance()->Update(dt);
		break;
	case IAP:
		IAPMenu::GetInstance()->Update(dt);
		break;
	case BOOSTMENU:
		BoostMenu::GetInstance()->Update(dt);
		break;
	case LEADERBOARD:
		LeaderBoardMenu::GetInstance()->Update(dt);
		break;
	case GAMEPLAYSELECTMENU:
		GamePlaySelection::GetInstance()->Update(dt);
		break;
	case LEVELSELECTIONMENU:
		LevelSelectionMenu::GetInstance()->Update(dt);
	default:
		break;
	}
#if ! USE_SKY_MENU
	if (!p_IsInActionPhase)
	{
		UpdateBG(dt);
	}
#endif
}
void MenuManager::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
#if ! USE_SKY_MENU
	if (!p_IsInActionPhase)
	{
		VisitBG(renderer,  transform,  flags, spriteBatch);
	}
#endif
	if (p_CurrentMenuLayer == MAINMENU)
	{
		MainMenu::GetInstance()->Visit(renderer,  transform,  flags,  spriteBatch );
	}
	else if (p_CurrentMenuLayer == OPTIONMENU)
	{
		MainMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
		OptionsMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == INFOMENU)
	{
		MainMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
		InfoMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LOADMENU)
	{
	}
	else if (p_CurrentMenuLayer == ACTION_PHASE)
	{
		HUDMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == IGM)
	{
		HUDMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
		IGMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == RESULTMENU)
	{
		ResultMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == IAP)
	{
		IAPMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == BOOSTMENU)
	{
		BoostMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LEADERBOARD)
	{
		MainMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
		LeaderBoardMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == GAMEPLAYSELECTMENU)
	{
		GamePlaySelection::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LEVELSELECTIONMENU)
	{
		LevelSelectionMenu::GetInstance()->Visit(renderer, transform, flags, spriteBatch);
	}
	if (p_IsPopUpAppear)
	{
		GetPopUp->Visit(renderer, transform, flags, spriteBatch);
	}
}
void MenuManager::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
#if ! USE_SKY_MENU
	if (!p_IsInActionPhase)
	{
		DrawBG(renderer, transform, flags, spriteBatch);
	}
#endif
	//
	if (p_CurrentMenuLayer == MAINMENU)
	{
		MainMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == OPTIONMENU)
	{
		MainMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
		OptionsMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == INFOMENU)
	{
		MainMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
		InfoMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LOADMENU)
	{
	}
	else if (p_CurrentMenuLayer == ACTION_PHASE)
	{
		HUDMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == IGM)
	{
		HUDMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
		IGMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == RESULTMENU)
	{
		ResultMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == IAP)
	{
		IAPMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == BOOSTMENU)
	{
		BoostMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LEADERBOARD)
	{
		MainMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
		LeaderBoardMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == GAMEPLAYSELECTMENU)
	{
		GamePlaySelection::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	else if (p_CurrentMenuLayer == LEVELSELECTIONMENU)
	{
		LevelSelectionMenu::GetInstance()->Draw(renderer, transform, flags, spriteBatch);
	}
	if (p_IsPopUpAppear)
	{
		GetPopUp->Draw(renderer, transform, flags, spriteBatch);
	}
}
void  MenuManager::SwitchToMenu(MENULAYER m)
{
	switch (m)
	{
	case MENU_NONE:
		break;
	case MAINMENU:
		MainMenu::GetInstance()->OnShow();
		break;
	case OPTIONMENU:
		OptionsMenu::GetInstance()->OnShow();
		break;
	case INFOMENU:
		InfoMenu::GetInstance()->OnShow();
		break;
	case LOADMENU:
		break;
	case ACTION_PHASE:
		HUDMenu::GetInstance()->OnShow();
		break;
	case IGM:
		IGMenu::GetInstance()->OnShow();
		break;
	case RESULTMENU:
		ResultMenu::GetInstance()->OnShow();
		break;
	case IAP:
		IAPMenu::GetInstance()->OnShow();
		break;
	case BOOSTMENU:
		BoostMenu::GetInstance()->OnShow();
		break;
	case LEADERBOARD:
		LeaderBoardMenu::GetInstance()->OnShow();
		break;
	case GAMEPLAYSELECTMENU:
		GamePlaySelection::GetInstance()->OnShow();
		break;
	case LEVELSELECTIONMENU:
		LevelSelectionMenu::GetInstance()->OnShow();
		break;
	default:
		break;
	}
}
void  MenuManager::CloseCurrentMenu(MENULAYER m)
{
	switch (m)
	{
	case MENU_NONE:
		break;
	case MAINMENU:
		MainMenu::GetInstance()->OnHide();
		break;
	case OPTIONMENU:
		OptionsMenu::GetInstance()->OnHide();
		break;
	case INFOMENU:
		InfoMenu::GetInstance()->OnHide();
		break;
	case LOADMENU:
		break;
	case ACTION_PHASE:
		HUDMenu::GetInstance()->OnHide();
		break;
	case IGM:
		IGMenu::GetInstance()->OnHide();
		break;
	case RESULTMENU:
		ResultMenu::GetInstance()->OnHide();
		break;
	case IAP:
		IAPMenu::GetInstance()->OnHide();
		break;
	case BOOSTMENU:
		BoostMenu::GetInstance()->OnHide();
		break;
	case LEADERBOARD:
		LeaderBoardMenu::GetInstance()->OnHide();
		break;
	case GAMEPLAYSELECTMENU:
		GamePlaySelection::GetInstance()->OnHide();
		break;
	case LEVELSELECTIONMENU:
		LevelSelectionMenu::GetInstance()->OnHide();
		break;
	default:
		break;
	}
}

void  MenuManager::SetHScore(int s, bool force)
{
	if (force == true || p_HScore != SaveMgr->GetDataSave()->GetHScore())
	{
		CC_ASSERT("HACK DETECTION!");
		p_HScore = 0;
		SaveMgr->GetDataSave()->SetHScore(0);
	}
	else
	{
		p_HScore = s;
		SaveMgr->GetDataSave()->SetHScore(p_HScore);
	}
}
int  MenuManager::GetHScore(bool force)
{
	if (force == true || p_HScore != SaveMgr->GetDataSave()->GetHScore())
	{
		CC_ASSERT("HACK DETECTION!");
		p_HScore = 0;
		return 0;
	}
	else
	{
		return p_HScore;
	}
}

void  MenuManager::SetCoin(int s, bool force)
{
	if (force == false && p_Coin != SaveMgr->GetDataSave()->GetCoin())
	{
		CC_ASSERT("HACK DETECTION!");
		p_Coin = 0;
		SaveMgr->GetDataSave()->SetCoin(0);
	}
	else
	{
		p_Coin = s;
		SaveMgr->GetDataSave()->SetCoin(p_Coin);
	}
}
int  MenuManager::Getcoin(bool force)
{
	if (force == false && p_Coin != SaveMgr->GetDataSave()->GetCoin())
	{
		CC_ASSERT("HACK DETECTION!");
		p_Coin = 0;
		return 0;
	}
	else
	{
		return p_Coin;
	}
}


void MenuManager::OnUpdatePopUp(float dt)
{
	GetPopUp->Update(dt);
}

void MenuManager::OnShowPopUp(std::string str, int numberBtn, std::string strCB, void * objectCallBack, void(*pt2Function)(void* pt2Object, std::string str))
{
	GetPopUp->OnShowPopUp(str, numberBtn, strCB, objectCallBack, pt2Function);
}

void MenuManager::OnHidePopUp()
{
	GetPopUp->OnHide();
}
void MenuManager::OnReloadString()
{
	MainMenu::GetInstance()->OnReloadString();
	OptionsMenu::GetInstance()->OnReloadString();
	InfoMenu::GetInstance()->OnReloadString();
	HUDMenu::GetInstance()->OnReloadString();
	IGMenu::GetInstance()->OnReloadString();
	ResultMenu::GetInstance()->OnReloadString();
	IAPMenu::GetInstance()->OnReloadString();
	BoostMenu::GetInstance()->OnReloadString();
	LeaderBoardMenu::GetInstance()->OnReloadString();
	GamePlaySelection::GetInstance()->OnReloadString();
	LevelSelectionMenu::GetInstance()->OnReloadString();
	GetPopUp->OnReloadString();
}

void MenuManager::HandleInterrupt()
{
	if (p_CurrentMenuLayer == ACTION_PHASE)
	{
		HUDMenu::GetInstance()->OnShowIGM();
	}
}

void MenuManager::UpdateBG(float dt)
{
	p_SkyObjectBG->onUpdate(dt);
}
void MenuManager::VisitBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	p_SkyObjectBG->onVisit(renderer, transform, flags, spriteBatch);
}
void MenuManager::DrawBG(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	p_SkyObjectBG->onDraw(renderer, transform, flags, spriteBatch);
}
void MenuManager::CheckAndRemoveBG()
{
	p_SkyObjectBG->onRemoveBG();
}

int MenuManager::OnUnlockLevelIdx(int idx)
{
	int res = LevelSelectionMenu::GetInstance()->OnUnlockLevel(idx);
	return res;
}

bool MenuManager::OnSetDetailAtLevelIdx(int idx, int star, int time)
{
	bool res = LevelSelectionMenu::GetInstance()->OnSetDetailAtLevel(idx, star, time);
	return res;
}

bool MenuManager::IsThisSessionUnlock(int idx_level)
{
	bool res = LevelSelectionMenu::GetInstance()->IsSessionUnlock(idx_level);
	return res;
}