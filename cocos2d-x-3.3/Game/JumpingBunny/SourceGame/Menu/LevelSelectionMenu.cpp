#include "LevelSelectionMenu.h"
#include "MainMenu.h"
#include "Utility/SoundManager.h"
#include "GamePlay/APManager.h"
using namespace cocos2d;

/*******************************************************************************************
																						   *
LEVEL SELECTION DETAIL																	   *
																						   *
																						   *
*******************************************************************************************/
//////////////////////////////////////////////////////////////
LevelSelectionDetail::LevelSelectionDetail()
{
	p_MainBackground = NULL;
	p_Star_0_bg = NULL;
	p_Star_1_bg = NULL;
	p_Star_2_bg = NULL;
	p_Star_3_bg = NULL;
	p_StateLock = NULL;

	p_DisplayTime = NULL;
	p_NumberIdx = NULL;


	p_TimePlay = 0;
	p_CurrentIdx = 0;
	p_CurentStarGain = 0;
	p_IsUnlock = false;

	p_DeltaPosNumberIdx = Vec2(0, 0);
	p_DeltaPosDisplayTime = Vec2(0, 0);
	p_DeltaPosStarBG = Vec2(0, 0);

	p_IsPress = false;
	p_ScaleNormal = Vec2(0,0);

	p_DistanceMove = 0.f;
}

//////////////////////////////////////////////////////////////
LevelSelectionDetail::~LevelSelectionDetail()
{
	CC_SAFE_RELEASE_NULL(p_MainBackground);
	CC_SAFE_RELEASE_NULL(p_Star_0_bg);
	CC_SAFE_RELEASE_NULL(p_Star_1_bg);
	CC_SAFE_RELEASE_NULL(p_Star_2_bg);
	CC_SAFE_RELEASE_NULL(p_Star_3_bg);
	CC_SAFE_RELEASE_NULL(p_StateLock);

	CC_SAFE_RELEASE_NULL(p_DisplayTime);
	CC_SAFE_RELEASE_NULL(p_NumberIdx);


	p_TimePlay = 0;
	p_CurrentIdx = 0;
	p_CurentStarGain = 0;
	p_IsUnlock = false;

	p_DeltaPosNumberIdx = Vec2(0, 0);
	p_DeltaPosDisplayTime = Vec2(0, 0);
	p_DeltaPosStarBG = Vec2(0, 0);

	p_DistanceMove = 0.f;
}

//////////////////////////////////////////////////////////////
void LevelSelectionDetail::Init(int idx, Vec2 Pos)
{
	p_CurrentIdx = idx;
	if (p_CurrentIdx == 0)
	{
		//always unlock the first level;
		p_IsUnlock = true;
	}

	//load the main background
	p_MainBackground = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("SqurePopUp")->getTexture(), TextureMgr->GetSpriteFromMemory("SqurePopUp")->getTextureRect());
	//set size of the main bg
	Vec2 sizeRatio = Vec2(133.f / p_MainBackground->getContentSize().width, 115.f / p_MainBackground->getContentSize().height) * GetScaleFactor();
	p_ScaleNormal = sizeRatio;
	p_MainBackground->setScale(sizeRatio.x);
	p_MainBackground->setPosition(Pos);
	p_MainBackground->retain();

	//init the font index menu
	int idxFont = 3;
	ResUILayout * res = new ResUILayout();
	res->BorderWid = 4.f;
	res->TintBorder[0] = 134; res->TintBorder[1] = 66; res->TintBorder[2] = 17; res->TintBorder[3] = 255;
	res->source = std::to_string(p_CurrentIdx + 1);
	res->ScaleRatio = Vec2(48, 48);
	p_DeltaPosNumberIdx = Vec2(0, 19)*GetScaleFactor();

	p_NumberIdx = new StringEntity();
	p_NumberIdx->SetString(TextureMgr->GetFontData(idxFont), res);
	p_NumberIdx->SetPosition(Pos + p_DeltaPosNumberIdx);//pos is 153-58 , the main is 153-77 so the delta is 0-19;
	p_NumberIdx->retain();


	//init the font time play display menu
	idxFont = 0;
	res->BorderWid = 1.f;
	res->TintBorder[0] = 166; res->TintBorder[1] = 106; res->TintBorder[2] = 2; res->TintBorder[3] = 255;
	res->source = convertToDisplayTime(p_TimePlay);
	res->ScaleRatio = Vec2(25, 25);
	p_DeltaPosDisplayTime = Vec2(0, -59)*GetScaleFactor();

	p_DisplayTime = new StringEntity();
	p_DisplayTime->SetString(TextureMgr->GetFontData(idxFont), res);
	p_DisplayTime->SetPosition(Pos + p_DeltaPosDisplayTime);//pos is 153-136 , the main is 153-77 so the delta is 0- -59;
	p_DisplayTime->retain();


	//init the lock status bg
	p_StateLock = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("bg_lock_level")->getTexture(), TextureMgr->GetSpriteFromMemory("bg_lock_level")->getTextureRect());
	//set size of the main bg
	sizeRatio = Vec2(100.f / p_StateLock->getContentSize().width, 100.f / p_StateLock->getContentSize().height) * GetScaleFactor();
	p_StateLock->setScale(sizeRatio.x);
	p_StateLock->setPosition(Pos);
	p_StateLock->retain();

	//init the the star level 0 
	p_Star_0_bg = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("bg_0_star")->getTexture(), TextureMgr->GetSpriteFromMemory("bg_0_star")->getTextureRect());
	//set size of the main bg
	sizeRatio = Vec2(112.f / p_Star_0_bg->getContentSize().width, 33.f / p_Star_0_bg->getContentSize().height) * GetScaleFactor();
	p_Star_0_bg->setScale(sizeRatio.x);//pos is 153-104 , the main is 153-77 so the delta is 0- -27;
	p_DeltaPosStarBG = Vec2(0, -27)*GetScaleFactor();
	p_Star_0_bg->setPosition(Pos + p_DeltaPosStarBG);
	p_Star_0_bg->retain();

	//init the the star level 1 
	p_Star_1_bg = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("bg_1_star")->getTexture(), TextureMgr->GetSpriteFromMemory("bg_1_star")->getTextureRect());
	//set size of the main bg
	sizeRatio = Vec2(112.f / p_Star_1_bg->getContentSize().width, 33.f / p_Star_1_bg->getContentSize().height) * GetScaleFactor();
	p_Star_1_bg->setScale(sizeRatio.x);
	p_Star_1_bg->setPosition(Pos + p_DeltaPosStarBG);
	p_Star_1_bg->retain();

	//init the the star level 2 
	p_Star_2_bg = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("bg_2_star")->getTexture(), TextureMgr->GetSpriteFromMemory("bg_2_star")->getTextureRect());
	//set size of the main bg
	sizeRatio = Vec2(112.f / p_Star_2_bg->getContentSize().width, 33.f / p_Star_2_bg->getContentSize().height) * GetScaleFactor();
	p_Star_2_bg->setScale(sizeRatio.x);
	p_Star_2_bg->setPosition(Pos + p_DeltaPosStarBG);
	p_Star_2_bg->retain();

	//init the the star level 3 
	p_Star_3_bg = Sprite::createWithTexture(TextureMgr->GetSpriteFromMemory("bg_3_star")->getTexture(), TextureMgr->GetSpriteFromMemory("bg_3_star")->getTextureRect());
	//set size of the main bg
	sizeRatio = Vec2(112.f / p_Star_3_bg->getContentSize().width, 33.f / p_Star_3_bg->getContentSize().height) * GetScaleFactor();
	p_Star_3_bg->setScale(sizeRatio.x);
	p_Star_3_bg->setPosition(Pos + p_DeltaPosStarBG);
	p_Star_3_bg->retain();
}

//////////////////////////////////////////////////////////////
Texture2D * LevelSelectionDetail::GetAtlasFont(int idx)
{
	if (idx == 0)
	{
		auto tx = p_DisplayTime->GetLabel()->getTexture();
		return tx;
	}
	else
	{
		auto tx = p_NumberIdx->GetLabel()->getTexture();
		return tx;
	}
	
}
//////////////////////////////////////////////////////////////
void LevelSelectionDetail::Update(float dt)
{

}

//////////////////////////////////////////////////////////////
void LevelSelectionDetail::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch, SpriteBatchNode * spriteBatch_1)
{
	p_MainBackground->visit(renderer, transform, flags);
	if (p_IsUnlock)
	{
		p_NumberIdx->VisitString(renderer, transform, flags, spriteBatch_1);

		if (p_CurentStarGain == 0)
		{
			p_Star_0_bg->visit(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 1)
		{
			p_Star_1_bg->visit(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 2)
		{
			p_Star_2_bg->visit(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 3)
		{
			p_Star_3_bg->visit(renderer, transform, flags);
		}
	}
	else
	{
		p_StateLock->visit(renderer, transform, flags);
	}
	p_DisplayTime->VisitString(renderer, transform, flags, spriteBatch);
}

//////////////////////////////////////////////////////////////
void LevelSelectionDetail::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch, SpriteBatchNode * spriteBatch_1)
{
	p_MainBackground->draw(renderer, transform, flags);
	if (p_IsUnlock)
	{
		p_NumberIdx->DrawString(renderer, transform, flags, spriteBatch_1);

		if (p_CurentStarGain == 0)
		{
			p_Star_0_bg->draw(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 1)
		{
			p_Star_1_bg->draw(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 2)
		{
			p_Star_2_bg->draw(renderer, transform, flags);
		}
		else if (p_CurentStarGain == 3)
		{
			p_Star_3_bg->draw(renderer, transform, flags);
		}
	}
	else
	{
		p_StateLock->draw(renderer, transform, flags);
	}
	p_DisplayTime->DrawString(renderer, transform, flags, spriteBatch);
}

//////////////////////////////////////////////////////////////
void LevelSelectionDetail::SetPosition(Point pos)
{
	p_MainBackground->setPosition(pos);
	p_NumberIdx->SetPosition(pos + p_DeltaPosNumberIdx);
	p_DisplayTime->SetPosition(pos + p_DeltaPosDisplayTime);
	p_StateLock->setPosition(pos);
	p_Star_0_bg->setPosition(pos + p_DeltaPosStarBG);
	p_Star_1_bg->setPosition(pos + p_DeltaPosStarBG);
	p_Star_2_bg->setPosition(pos + p_DeltaPosStarBG);
	p_Star_3_bg->setPosition(pos + p_DeltaPosStarBG);
}

//////////////////////////////////////////////////////////////
Point LevelSelectionDetail::GetPosition()
{
	return p_MainBackground->getPosition();
}

//////////////////////////////////////////////////////////////
void LevelSelectionDetail::reloadString()
{
	p_NumberIdx->ReloadString();
	p_DisplayTime->ReloadString();
}

/*******************************************************************************************
																						   *
MENU SELECTION LEVEL																	   *
																						   *
																						   *
*******************************************************************************************/


LevelSelectionMenu * LevelSelectionMenu::m_Instance = NULL;

//////////////////////////////////////////////////////////////
LevelSelectionMenu::LevelSelectionMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IdxMenuData = 12;
	IsFinishCreate = true;
	p_CurrentState = STATE_MOVE_LEVEL::LEVEL_IDLE;
	p_CurrentLevelPageIdx = 0;

	p_timeQuickMove = 0.f;
	p_originTimeQuickMove = p_timeQuickMove ;
	p_DeltaTimeQuickMove = 1.f;

}

//////////////////////////////////////////////////////////////
LevelSelectionMenu::~LevelSelectionMenu()
{
	p_ListLevel.clear();

	IsFinishCreate = true;
	p_CurrentState = STATE_MOVE_LEVEL::LEVEL_IDLE;
	p_CurrentLevelPageIdx = 0;

	p_timeQuickMove = 0.f;
	p_originTimeQuickMove = p_timeQuickMove;
	p_DeltaTimeQuickMove = 1.f;

	p_font_1 = NULL;
	p_font_2 = NULL;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	//init star per session
	for (int i = 0; i < (NUMBER_LEVEL / NUMBER_LEVEL_PER_SESSION); i++)
	{
		InitSession( i == 0);
	}

	//auto set the page base as the save
	int mx = SaveMgr->GetMaxLevelReach();
	int current_page_reach = 0;
	if (mx > 0)
	{
		current_page_reach = mx % NUMBER_LEVEL_PER_PAGE == 0 ? (mx / NUMBER_LEVEL_PER_PAGE) - 1 : (mx / NUMBER_LEVEL_PER_PAGE);
		p_CurrentLevelPageIdx = current_page_reach;
	}
	for (int i = 0; i < NUMBER_LEVEL; i++)
	{
		float threadHold = 1;
#ifdef USE_HALF_LEVEL
		threadHold = 1.5f;
#endif
		LevelSelectionDetail * level = new LevelSelectionDetail();
		//set The Position
		//check the current page

		int cur_page_idx = i / NUMBER_LEVEL_PER_PAGE;
		//
		int j = i - cur_page_idx * NUMBER_LEVEL_PER_PAGE;
		//the raw is 4 and the colum is 3
		//the first is 153-77 , the last is 650-393, distance in w is 161, distance in h is 155 (all number is calculate with 800x480)
		int addition_pos_x = (GetWinSize().width * (cur_page_idx - current_page_reach));
		//check the row
		int cur_row = j / NUMBER_LEVEL_ROW;
		//check the colume
		int cur_colum = j % NUMBER_LEVEL_ROW;
		//
		float posX = (153 * threadHold + cur_colum * 161) * GetScaleFactor();
		float posy = GetWinSize().height - ((77 * threadHold + cur_row * 155 ) * GetScaleFactor(true));
		//
		Vec2 FinalPos = Vec2(posX + addition_pos_x, posy);
		//
		level->Init(i, FinalPos);
		if (!p_font_1)
		{
			//p_font_1 = SpriteBatchNode::createWithTexture(level->GetAtlasFont(0), NUMBER_LEVEL);
			//p_font_1->retain();
		}
		if (!p_font_2)
		{
			//p_font_2 = SpriteBatchNode::createWithTexture(level->GetAtlasFont(1), NUMBER_LEVEL);
			//p_font_2->retain();
		}
		level->retain();
		//
		//load from file
		auto lvlgp = SaveMgr->GetLevelWithName(std::string("part_") + std::to_string(i));
		if (lvlgp)
		{
			level->SetUnlock();
			level->SetStarGain(lvlgp->star_gain);
			level->SetTimePlay(lvlgp->timePlay);
			//
			int idx_star = i / NUMBER_LEVEL_PER_SESSION;
			AddStarSession(idx_star, lvlgp->star_gain);
		}

		//
		p_ListLevel.pushBack(level);
		//
		CC_SAFE_RELEASE_NULL(level);
	}
	//
	SetStringSession();
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::Update(float dt)
{
	p_timeQuickMove -= p_DeltaTimeQuickMove * 100.f;
	if (p_timeQuickMove <= 0)
	{
		p_timeQuickMove = 0;
		//
		if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_LEFT || p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_RIGHT)
		{
			SetStringSession();
		}
		//
		p_CurrentState = STATE_MOVE_LEVEL::LEVEL_IDLE;
		//
		onUpdate(dt);
		OnUpdatePress();
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

	if (p_timeQuickMove > 0)
	{
		if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_LEFT)
		{
			for (int i = 0; i < NUMBER_LEVEL; i++)
			{
				Point cur_pos = p_ListLevel.at(i)->GetPosition();
				float delta = ((p_DeltaTimeQuickMove * 100.f / p_originTimeQuickMove) *GetWinSize().width);
				p_ListLevel.at(i)->SetDistanceMove(p_ListLevel.at(i)->GetDistanceMove() + delta);
				cur_pos.x += delta;
				if (p_ListLevel.at(i)->GetDistanceMove() >= GetWinSize().width)
				{
					float pre_delta = p_ListLevel.at(i)->GetDistanceMove() - GetWinSize().width;
					p_ListLevel.at(i)->SetDistanceMove(0);
					p_timeQuickMove = 0;
					cur_pos.x -= pre_delta;
				}
				p_ListLevel.at(i)->SetPosition(cur_pos);
			}
		}
		else if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_RIGHT)
		{
			for (int i = 0; i < NUMBER_LEVEL; i++)
			{
				Point cur_pos = p_ListLevel.at(i)->GetPosition();
				float delta = ((p_DeltaTimeQuickMove * 100.f / p_originTimeQuickMove) *GetWinSize().width);
				p_ListLevel.at(i)->SetDistanceMove(p_ListLevel.at(i)->GetDistanceMove() + delta);
				cur_pos.x -= delta;
				if (p_ListLevel.at(i)->GetDistanceMove() >= GetWinSize().width)
				{
					float pre_delta = p_ListLevel.at(i)->GetDistanceMove() - GetWinSize().width;
					p_ListLevel.at(i)->SetDistanceMove(0);
					p_timeQuickMove = 0;
					cur_pos.x += pre_delta;
				}
				p_ListLevel.at(i)->SetPosition(cur_pos);
			}
		}
	}
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer, transform, flags, spriteBatch);
	if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_LEFT || p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_RIGHT)
	{
		for (int i = 0; i < NUMBER_LEVEL; i++)
		{
			p_ListLevel.at(i)->Visit(renderer, transform, flags, p_font_1, p_font_2);
		}
	}
	else if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_IDLE)
	{
		int pageIdxMax = (p_CurrentLevelPageIdx + 1) * NUMBER_LEVEL_PER_PAGE;
		if (p_CurrentLevelPageIdx >= (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) )
		{
			pageIdxMax = NUMBER_LEVEL;
		}

		for (int i = p_CurrentLevelPageIdx * NUMBER_LEVEL_PER_PAGE; i < pageIdxMax; i++)
		{
			p_ListLevel.at(i)->Visit(renderer, transform, flags, p_font_1, p_font_2);
		}
	}
	if (p_font_1)
	{
		p_font_1->visit(renderer, transform, flags);
	}
	if (p_font_2)
	{
		p_font_2->visit(renderer, transform, flags);
	}
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	if (p_font_1)
	{
		p_font_1->removeAllChildrenWithCleanup(true);
	}
	if (p_font_2)
	{
		p_font_2->removeAllChildrenWithCleanup(true);
	}

	onDraw(renderer, transform, flags,spriteBatch);
	if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_LEFT || p_CurrentState == STATE_MOVE_LEVEL::LEVEL_MOVE_RIGHT)
	{
		for (int i = 0; i < NUMBER_LEVEL; i++)
		{
			p_ListLevel.at(i)->Draw(renderer, transform, flags, p_font_1, p_font_2);
		}
	}
	else if (p_CurrentState == STATE_MOVE_LEVEL::LEVEL_IDLE)
	{
		int pageIdxMax = (p_CurrentLevelPageIdx + 1) * NUMBER_LEVEL_PER_PAGE;
		if (p_CurrentLevelPageIdx >= (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) )
		{
			pageIdxMax = NUMBER_LEVEL;
		}

		for (int i = p_CurrentLevelPageIdx * NUMBER_LEVEL_PER_PAGE; i < pageIdxMax; i++)
		{
			p_ListLevel.at(i)->Draw(renderer, transform, flags, p_font_1, p_font_2);
		}
	}

	if (p_font_1)
	{
		p_font_1->draw(renderer, transform, flags);
	}
	if (p_font_2)
	{
		p_font_2->draw(renderer, transform, flags);
	}
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnProcessButton(std::string name)
{
	if (p_timeQuickMove > 0) //when quick move is in processing, do not perform any button
		return;
	if (name == "btn_move_left")
	{
		p_CurrentLevelPageIdx--;
		if (p_CurrentLevelPageIdx <= -1)
		{
			p_CurrentLevelPageIdx = 0;
			p_CurrentState = STATE_MOVE_LEVEL::LEVEL_IDLE;
		}
		else
		{
			p_CurrentState = STATE_MOVE_LEVEL::LEVEL_MOVE_LEFT;
			SetTimeQuickMove();

		}
	}
	else if (name == "btn_move_right")
	{
		p_CurrentLevelPageIdx++;
		int maxPage = (NUMBER_LEVEL % NUMBER_LEVEL_PER_PAGE == 0) ? (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) : (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) + 1;
		if (p_CurrentLevelPageIdx >= maxPage)
		{
			p_CurrentLevelPageIdx = (NUMBER_LEVEL % NUMBER_LEVEL_PER_PAGE == 0) ? (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) - 1 : (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE);
			p_CurrentState = STATE_MOVE_LEVEL::LEVEL_IDLE;
		}
		else
		{
			p_CurrentState = STATE_MOVE_LEVEL::LEVEL_MOVE_RIGHT;
			SetTimeQuickMove();
		}

	}
	else if (name == "CloseBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::GAMEPLAYSELECTMENU;
	}

}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::LEVELSELECTIONMENU);
	OnFadeIn();
	//
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnHide()
{
	OnFadeOut();
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::CreateInitGamePlay()
{
	IsFinishCreate = false;
	APMgr->InitWithGamePlay(APMgr->GetCurrentGamePlay());
	IsFinishCreate = true;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::SetTimeQuickMove()
{
	p_timeQuickMove = 10.f;
	p_originTimeQuickMove = p_timeQuickMove;
	p_DeltaTimeQuickMove = 1.f;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnCheckPressWithLevel()
{
	int pageIdxMax = (p_CurrentLevelPageIdx + 1) * NUMBER_LEVEL_PER_PAGE;
	if (p_CurrentLevelPageIdx >= (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE))
	{
		pageIdxMax = NUMBER_LEVEL;
	}
	int beginIdx = p_CurrentLevelPageIdx * NUMBER_LEVEL_PER_PAGE;
	if (InputMgr->GetNumberTouch() > 0)
	{
		auto p = InputMgr->GetPosAtId(0);
		for (int i = beginIdx; i < pageIdxMax; i++)
		{
			auto p = p_ListLevel.at(i)->GetPosition();
			auto s = p_ListLevel.at(i)->GetNormalSize();
			Rect rec(p.x - s.width / 2, p.y - s.height / 2, s.width, s.height);
			if (InputMgr->IsPressOnButton(rec))
			{
				p_ListLevel.at(i)->IsPress(true);
				InputMgr->SetIsTouchOnScreen();
#if USE_CC_AUDIO
				if (p_IsPlaySoundPress)
				{
					GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_CLICK"));
					p_IsPlaySoundPress = false;
				}
#endif
			}
			else
			{
				p_ListLevel.at(i)->IsPress(false);
			}
		}
	}
	else
	{
		for (int i = beginIdx; i < pageIdxMax; i++)
		{
			if (p_ListLevel.at(i)->IsPress() == true)
			{
				auto p = p_ListLevel.at(i)->GetPosition();
				auto s = p_ListLevel.at(i)->GetNormalSize();
				Rect rec(p.x - s.width / 2, p.y - s.height / 2, s.width, s.height);
				if (InputMgr->IsReleaseOnbutton(rec))
				{
					OnProcessLevelIdx(i);
				}
				InputMgr->OnClearTouchReleaseList();
			}
			p_ListLevel.at(i)->IsPress(false);
		}
		p_IsPlaySoundPress = true;
	}
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnUpdatePress()
{
	OnCheckPressWithLevel();

	int pageIdxMax = (p_CurrentLevelPageIdx + 1) * NUMBER_LEVEL_PER_PAGE;
	if (p_CurrentLevelPageIdx >= (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE))
	{
		pageIdxMax = NUMBER_LEVEL;
	}
	int beginIdx = p_CurrentLevelPageIdx * NUMBER_LEVEL_PER_PAGE;

	for (int i = beginIdx; i < pageIdxMax; i++)
	{
		p_ListLevel.at(i)->UpdatePress();
	}
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnProcessLevelIdx(int idx)
{
	int pageIdxMax = (p_CurrentLevelPageIdx + 1) * NUMBER_LEVEL_PER_PAGE;
	if (p_CurrentLevelPageIdx >= (int)(NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE))
	{
		pageIdxMax = NUMBER_LEVEL;
	}
	int beginIdx = p_CurrentLevelPageIdx * NUMBER_LEVEL_PER_PAGE;
	//check idx on the range
	if (idx >= NUMBER_LEVEL || idx < beginIdx || idx >= pageIdxMax)
		return;
	//check unlock or no
	if (!p_ListLevel.at(idx)->IsUnlock())
	{
		MenuMgr->OnShowPopUp("THIS LEVEL'S STILL LOCKED !\nCOMPLETE PREVIOUS TO UNLOCK IT", 1);
		return;
	}
	//process to gameplay

	OnHide();
#if USE_CC_AUDIO
	GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_PLAY_CLICK"));
#endif
	p_MenuToSwitchTo = MENULAYER::ACTION_PHASE;
	APMgr->SetCurrentGamePlay(TYPE_GAMEPLAY::GAMEPLAY_PLAY_STORY);
	APMgr->SetCurrentPartPlay(idx);
#if USE_CREATE_GAMEPLAY_IN_THREAD
	IsFinishCreate = false;
	p_threadCreateGame = std::thread(&LevelSelectionMenu::CreateInitGamePlay, this);
#else
	IsFinishCreate = true;
	GamePlaySelection::CreateInitGamePlay();
#endif

}

//////////////////////////////////////////////////////////////
int LevelSelectionMenu::OnUnlockLevel(int idx)
{
	if (idx >= p_ListLevel.size())
		return false;
	if (p_ListLevel.at(idx)->IsUnlock())
		return false;
	//
	int _current_session = idx / NUMBER_LEVEL_PER_SESSION;
	if (p_currentSession.at(_current_session)->IsUnlock)
	{
		p_ListLevel.at(idx)->SetUnlock();
		return 0;
	}
	else
	{
		if (p_currentSession.at(_current_session - 1)->CurrentStar >= (NUMBER_LEVEL_PER_SESSION - 1) * MAXIMUM_STAR)
		{
			p_ListLevel.at(idx)->SetUnlock();
			UnlockSession(_current_session, true);
			SetStringSession();
			return 0;
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////
bool LevelSelectionMenu::OnSetDetailAtLevel(int idx, int star, int time)
{
	bool res = false;
	if (idx >= p_ListLevel.size())
		return false;
	if (!p_ListLevel.at(idx)->IsUnlock())
		return false;
	if (star >= p_ListLevel.at(idx)->GetStarGain())
	{
		// add star gain more
		if (star > p_ListLevel.at(idx)->GetStarGain())
		{
			int _current_session = idx / NUMBER_LEVEL_PER_SESSION;
			AddStarSession(_current_session, star - p_ListLevel.at(idx)->GetStarGain());
			SetStringSession();
		}
		//
		p_ListLevel.at(idx)->SetStarGain(star);
		res = true;
		if (time < p_ListLevel.at(idx)->GetTimePlay() && p_ListLevel.at(idx)->GetTimePlay() != 0)
		{
			p_ListLevel.at(idx)->SetTimePlay(time);
			res = true;
		}

	}
	return res;
}

//////////////////////////////////////////////////////////////
void LevelSelectionMenu::OnReloadString()
{
	for (int i = 0; i < p_ListLevel.size(); i++)
	{
		p_ListLevel.at(i)->reloadString();
	}
}
//////////////////////////////////////////////////////////////
void LevelSelectionMenu::SetStringSession()
{
	int _current_session = (p_CurrentLevelPageIdx / (NUMBER_LEVEL_PER_SESSION / NUMBER_LEVEL_PER_PAGE)) + 1;
	std::string str = "session " + std::to_string(_current_session);
	std::string str1 = "";
	if (p_currentSession.at(_current_session - 1)->IsUnlock)
	{
		str1 = "you currently gain " + std::to_string(p_currentSession.at(_current_session - 1)->CurrentStar) + "/" + std::to_string(NUMBER_LEVEL_PER_SESSION * MAXIMUM_STAR) + " stars";
	}
	else
	{
		str1 = "this session is locked, you need at least " + std::to_string((NUMBER_LEVEL_PER_SESSION - 1) * MAXIMUM_STAR) + " stars \n you have: " + std::to_string(p_currentSession.at(_current_session - 2)->CurrentStar) + "/" + std::to_string(NUMBER_LEVEL_PER_SESSION * MAXIMUM_STAR) + " stars";
	}
	GetString("Text0")->SetStringName(str);
	GetString("Text1")->SetStringName(str1);

	//
	if (p_CurrentLevelPageIdx <= 0)
	{
		GetButtonByName("btn_move_left")->Visible(false);
		GetButtonByName("btn_move_right")->Visible(true);
	}
	else if (p_CurrentLevelPageIdx >= (NUMBER_LEVEL / NUMBER_LEVEL_PER_PAGE) - 1)
	{
		GetButtonByName("btn_move_left")->Visible(true);
		GetButtonByName("btn_move_right")->Visible(false);
	}
	else
	{
		GetButtonByName("btn_move_left")->Visible(true);
		GetButtonByName("btn_move_right")->Visible(true);
	}
}
//
void LevelSelectionMenu::UnlockSession(int idx, bool b)
{
	p_currentSession.at(idx)->IsUnlock = b;
}
//
void LevelSelectionMenu::AddStarSession(int idx, int star)
{
	if (!p_currentSession.at(idx)->IsUnlock)
	{
		return;
	}
	//
	int _star = p_currentSession.at(idx)->CurrentStar + star;
	p_currentSession.at(idx)->CurrentStar = _star;
	//
	if (p_currentSession.at(idx)->CurrentStar >= (NUMBER_LEVEL_PER_SESSION - 1) * MAXIMUM_STAR)
	{
		p_currentSession.at(idx + 1)->IsUnlock = true;
	}
}
//
void LevelSelectionMenu::InitSession(bool b, int star)
{
	p_currentSession.push_back(new SESSION_STATE(b,star));
}
//
bool LevelSelectionMenu::IsSessionUnlock(int idx)
{
	return p_currentSession.at(idx)->IsUnlock;
}