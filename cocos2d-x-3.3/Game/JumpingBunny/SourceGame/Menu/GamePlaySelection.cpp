#include "GamePlaySelection.h"
#include "MainMenu.h"
#include "Utility/SoundManager.h"
#include "GamePlay/APManager.h"
using namespace cocos2d;

GamePlaySelection * GamePlaySelection::m_Instance = NULL;

GamePlaySelection::GamePlaySelection()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IdxMenuData = 11;
	IsFinishCreate = true;
}


GamePlaySelection::~GamePlaySelection()
{
}

void GamePlaySelection::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void GamePlaySelection::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
#if USE_CREATE_GAMEPLAY_IN_THREAD
			if (IsFinishCreate)
#endif
			{
				MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
				//reset object posion to make sure the menu not stuck
				ResetPositionResource();
#if USE_CREATE_GAMEPLAY_IN_THREAD
				if (p_threadCreateGame.joinable())
				{
					p_threadCreateGame.join();
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
		else if (p_curStatus == STATUS_MENU::M_FADEIN)
		{
		
		}
		p_curStatus = STATUS_MENU::M_IDLE;
		if (IsBackKeyPress())
		{
			OnProcessButton("CloseBtn");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}

void GamePlaySelection::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisit(renderer, transform, flags, spriteBatch);
}
void GamePlaySelection::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags,spriteBatch);
}

void GamePlaySelection::OnProcessButton(std::string name)
{
	if (name == "PlayBtn")
	{
		OnHide();
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_PLAY_CLICK"));
#endif
		p_MenuToSwitchTo = MENULAYER::LEVELSELECTIONMENU;
		IsFinishCreate = true;
	}
	else if (name == "TimePlayBtn")
	{
		OnHide();
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_PLAY_CLICK"));
#endif
		p_MenuToSwitchTo = MENULAYER::ACTION_PHASE;
		APMgr->SetCurrentGamePlay(TYPE_GAMEPLAY::GAMEPLAY_TIME_RUN);
#if USE_CREATE_GAMEPLAY_IN_THREAD
		IsFinishCreate = false;
		p_threadCreateGame = std::thread(&GamePlaySelection::CreateInitGamePlay,this);
#else
		IsFinishCreate = true;
		GamePlaySelection::CreateInitGamePlay();
#endif
	}
	else if (name == "CloseBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
		MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::GAMEPLAYSELECTMENU);
	}

}

void GamePlaySelection::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void GamePlaySelection::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void GamePlaySelection::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::GAMEPLAYSELECTMENU);
	OnFadeIn();
	//
}
void GamePlaySelection::OnHide()
{
	OnFadeOut();
}

void GamePlaySelection::CreateInitGamePlay()
{
	IsFinishCreate = false;
	APMgr->InitWithGamePlay(APMgr->GetCurrentGamePlay());
	IsFinishCreate = true;
}