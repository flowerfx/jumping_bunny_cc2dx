#include "LeaderBoardMenu.h"
#include "MainMenu.h"
using namespace cocos2d;

LeaderBoardMenu * LeaderBoardMenu::m_Instance = NULL;

LeaderBoardMenu::LeaderBoardMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IsDisplayGlobalScore = true;
	numberDisplayScore = 4;

	p_IdxMenuData = 9;
    
    p_Avatar = NULL;
}


LeaderBoardMenu::~LeaderBoardMenu()
{
    p_Avatar = NULL;
}

void LeaderBoardMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
	//
#if defined WP8 || defined ANDROID
	GetButtonByName("btn_facebook")->Visible(true);
	GetButtonByName("btn_gamecenter")->Visible(false);
#elif defined IPHONE
	GetButtonByName("btn_facebook")->Visible(false);
	GetButtonByName("btn_gamecenter")->Visible(true);
#else
	GetButtonByName("btn_facebook")->Visible(false);
	GetButtonByName("btn_gamecenter")->Visible(false);
#endif
	//temporary hide some btn
	GetButtonByName("btn_facebook")->Visible(false);
	GetButtonByName("btn_gamecenter")->Visible(false);
}


void LeaderBoardMenu::Update(float dt)
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
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f, true);
	}
}
void LeaderBoardMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer,  transform, flags,spriteBatch );
	if (p_DeltaMoveTime <= 0)
	{
		if (p_Avatar)
		{
			p_Avatar->visit();
		}
		if (p_IsDisplayGlobalScore && p_ListAvatarGlobal.size() > 0)
		{
			for (int i = 0; i < p_ListAvatarGlobal.size(); i++)
			{
				p_ListAvatarGlobal.at(i)->visit();
			}
		}
		else if (!p_IsDisplayGlobalScore && p_ListAvatarFriends.size() > 0)
		{
			for (int i = 0; i < p_ListAvatarFriends.size(); i++)
			{
				p_ListAvatarFriends.at(i)->visit();
			}
		}
	}
}
void LeaderBoardMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags,spriteBatch);
	if (p_DeltaMoveTime <= 0)
	{
		if (p_Avatar)
		{
			p_Avatar->draw(renderer, transform, flags);
		}
		if (p_IsDisplayGlobalScore && p_ListAvatarGlobal.size() > 0)
		{
			for (int i = 0; i < p_ListAvatarGlobal.size(); i++)
			{
				p_ListAvatarGlobal.at(i)->draw(renderer, transform, flags);
			}
		}
		else if (!p_IsDisplayGlobalScore && p_ListAvatarFriends.size() > 0)
		{
			for (int i = 0; i < p_ListAvatarFriends.size(); i++)
			{
				p_ListAvatarFriends.at(i)->draw(renderer, transform, flags);
			}
		}
	}
}

void LeaderBoardMenu::OnProcessButton(std::string name)
{
	if (name == "BackBtn")
	{
		OnHide();
		p_MenuToSwitchTo = MENULAYER::MAINMENU;
	}
	else if (name == "btn_gamecenter")
	{
		//todo
	}
	else if (name == "btn_facebook")
	{
		//todo
	}
	else if (name == "btn_global")
	{
		//todo
	}
}

void LeaderBoardMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void LeaderBoardMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void LeaderBoardMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::LEADERBOARD);
	//displayer user score
	OnParseUserScore();
	//displayer global leaderboard
	DisplayNameAndScoreGlobal();
	//
	OnFadeIn();
}
void LeaderBoardMenu::OnHide()
{
	OnFadeOut();
}

void LeaderBoardMenu::OnParseUserScore()
{
	FXSocialService::ScoreType * user = FXSocialService::SocialMgr->GetLeaderB()->CurrentUserScore();
	std::wstring name = user->Name != L"" ? user->Name : L"...";
	size_t score =		user->Score;
	p_DisplayString.find("Text15")->second->SetStringName(std::string(CCUnicodeToUtf8(name.data())));
	p_DisplayString.find("Text16")->second->SetStringName(std::to_string(score));
}
//display profile fb image
void LeaderBoardMenu::DisplayProfileFB(std::wstring name, std::string imgPath, std::string namesource, Point pos, Point sizeDesign, Sprite *(&sp))
{
	p_DisplayString.at(namesource)->SetStringName(std::string(CCUnicodeToUtf8(name.data())));
	//
	if (!sp)
	{
		sp = Sprite::create(imgPath);
		if (!sp)
			return;
		sp->retain();
	}
	else
	{
		sp = Sprite::create(imgPath);
	}
	auto winsize = GetWinSize();
	sp->setPosition(pos.x / 800.f * winsize.width, (480.f - pos.y) / 480.f * winsize.height);
	Point p_scale(1, 1);
	p_scale.x = (sizeDesign.x * (winsize.width / 800.f)) / sp->getContentSize().width;
	p_scale.y = (sizeDesign.y * (winsize.height / 480.f)) / sp->getContentSize().height;
	sp->setScaleX(p_scale.x);
	sp->setScaleY(p_scale.y);

}
//display profile fb user
void LeaderBoardMenu::DisplayProfileFBUser(std::wstring name, std::string imgPath)
{
	DisplayProfileFB(name, imgPath, "Text15", Point(736, 427), Point(64, 64), p_Avatar);
}
//display list player global score
void LeaderBoardMenu::DisplayProfileFBGlobal(std::vector<std::wstring> names, std::vector<std::string> imgPaths)
{
	CCASSERT(names.size() <= numberDisplayScore, "size is not enough!");
	CCASSERT(imgPaths.size() <= numberDisplayScore, "size is not enough!");

	//parse no 1
	if (imgPaths.size() <= 0)
	{
		return;
	}
	if (p_ListAvatarGlobal.size() <= 0)
	{
		p_ListAvatarGlobal.push_back(NULL);
	}
	DisplayProfileFB(names[0], imgPaths[0], "Text4", Point(736, 103), Point(64, 64), p_ListAvatarGlobal[0]);
	//parse no 2
	if (imgPaths.size() <= 1)
	{
		return;
	}
	if (p_ListAvatarGlobal.size() <= 1)
	{
		p_ListAvatarGlobal.push_back(NULL);
	}
	DisplayProfileFB(names[1], imgPaths[1], "Text7", Point(736, 175), Point(64, 64), p_ListAvatarGlobal[1]);
	//parse no 3
	if (imgPaths.size() <= 2)
	{
		return;
	}
	if (p_ListAvatarGlobal.size() <= 2)
	{
		p_ListAvatarGlobal.push_back(NULL);
	}
	DisplayProfileFB(names[2], imgPaths[2], "Text10", Point(736, 247), Point(64, 64), p_ListAvatarGlobal[2]);
	//parse no 4
	if (imgPaths.size() <= 3)
	{
		return;
	}
	if (p_ListAvatarGlobal.size() <= 3)
	{
		p_ListAvatarGlobal.push_back(NULL);
	}
	DisplayProfileFB(names[3], imgPaths[3], "Text13", Point(736, 319), Point(64, 64), p_ListAvatarGlobal[3]);
}
void LeaderBoardMenu::DisplayNameAndScoreGlobal()
{
	auto data = FXSocialService::SocialMgr->GetLeaderB()->GetTopPlayerScores();
	if (data.size() <= 0)
	{
		return;
	}
	p_DisplayString.at("Text4")->SetStringName(std::string(CCUnicodeToUtf8(data[0]->Name.data())));
	p_DisplayString.at("Text5")->SetStringName(std::to_string(data[0]->Score));
	if (data.size() <= 1)
	{
		return;
	}
	p_DisplayString.at("Text7")->SetStringName(std::string(CCUnicodeToUtf8(data[1]->Name.data())));
	p_DisplayString.at("Text8")->SetStringName(std::to_string(data[1]->Score));
	if (data.size() <= 2)
	{
		return;
	}
	p_DisplayString.at("Text10")->SetStringName(std::string(CCUnicodeToUtf8(data[2]->Name.data())));
	p_DisplayString.at("Text11")->SetStringName(std::to_string(data[2]->Score));
	if (data.size() <= 3)
	{
		return;
	}
	p_DisplayString.at("Text13")->SetStringName(std::string(CCUnicodeToUtf8(data[3]->Name.data())));
	p_DisplayString.at("Text14")->SetStringName(std::to_string(data[3]->Score));
}
//display list player friends score
void LeaderBoardMenu::DisplayProfileFBFriends(std::vector<std::wstring> names, std::vector<std::string> imgPaths)
{

}