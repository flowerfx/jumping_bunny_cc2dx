#include "MainMenu.h"
#include "HUDMenu.h"
#include "BoostMenu.h"
#include "GamePlay/ManageObjectAP.h"
#include "GamePlay/APManager.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
using namespace cocos2d;

BoostMenu * BoostMenu::m_Instance = NULL;

BoostMenu::BoostMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_IdxMenuData = 8;
}


BoostMenu::~BoostMenu()
{
}

void BoostMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void BoostMenu::Update(float dt)
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
			OnProcessButton("btn_ok");
			SetBackkey(false);
		}
	}
	else
	{
		float initDeltaTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
		UpdateFadeProcessItem(initDeltaTime, dt * 100.f);
	}
}
void BoostMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onVisit(renderer, transform, flags, spriteBatch);
}
void BoostMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}
void BoostMenu::OnProcess(std::string str)
{
	if (str == "callIAPMenu")
	{
		OnHide();
		if (!MenuMgr->IsInActionPhase())
		{
			p_MenuToSwitchTo = MENULAYER::IAP;
		}
		else
		{
			HUDMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IGM);
			p_MenuToSwitchTo = MENULAYER::IAP;
		}
	}
	else if (str == "process_buy_" + ListItemName[0])
	{
		OnProcessButton("btn_buy_1");
	}
	else if (str == "process_buy_" + ListItemName[1])
	{
		OnProcessButton("btn_buy_2");
	}
	else if (str == "process_buy_" + ListItemName[2])
	{
		OnProcessButton("btn_buy_3");
	}

}
void BoostMenu::OnProcessButton(std::string name)
{
	if (name == "btn_ok")
	{
		OnHide();
		if (!MenuMgr->IsInActionPhase())
		{
			p_MenuToSwitchTo = MENULAYER::MAINMENU;
		}
		else
		{
			HUDMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IGM);
			p_MenuToSwitchTo = MENULAYER::ACTION_PHASE;
		}
	}
	else if (name == "btn_buy_1")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		if (IsCanBuy(ListItemName[0]))
		{
			ProcessBuy(ListItemName[0]);
		}
	}
	else if (name == "btn_buy_2")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		if (IsCanBuy(ListItemName[1]))
		{
			ProcessBuy(ListItemName[1]);
		}
	}
	else if (name == "btn_buy_3")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		if (IsCanBuy(ListItemName[2]))
		{
			ProcessBuy(ListItemName[2]);
		}
	}
	else if (name == "btn_use_1")
	{
		if (APMgr->IsUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST))
		{
			ProcessUse(ListItemName[0], false);
			APMgr->SetUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST, false);
			p_DisplayString.find("tf_use_1")->second->SetStringName("USE");
		}
		else
		{
			if (!APMgr->IsOnStoryMode())
			{
				MenuMgr->OnShowPopUp("CAN'T USE THIS ITEM \n ON THIS GAME MODE!", 1);
				return;
			}

			if (IsCanUse(ListItemName[0]) )
			{
				ProcessUse(ListItemName[0]);
				APMgr->SetUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST, true);
				p_DisplayString.find("tf_use_1")->second->SetStringName("USING..");
			}
		}
	}
	else if (name == "btn_use_2")
	{
		if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
		{
			ProcessUse(ListItemName[1], false);
			APMgr->SetUseTheBoost(TYPE_BOOST::B_JUMP_BOOST, false);
			p_DisplayString.find("tf_use_2")->second->SetStringName("USE");
		}
		else
		{
			if (APMgr->IsOnStoryMode())
			{
				MenuMgr->OnShowPopUp("CAN'T USE THIS ITEM \n ON THIS GAME MODE!", 1);
				return;
			}

			if (IsCanUse(ListItemName[1]))
			{
				ProcessUse(ListItemName[1]);
				APMgr->SetUseTheBoost(TYPE_BOOST::B_JUMP_BOOST, true);
				p_DisplayString.find("tf_use_2")->second->SetStringName("USING..");
			}
		}
	}
	else if (name == "btn_use_3")
	{
		if (APMgr->IsUseTheBoost(TYPE_BOOST::B_LIFE_BOOST))
		{
			ProcessUse(ListItemName[2],false);
			APMgr->SetUseTheBoost(TYPE_BOOST::B_LIFE_BOOST, false);
			p_DisplayString.find("tf_use_3")->second->SetStringName("USE");
		}
		else
		{
			if (APMgr->IsOnStoryMode())
			{
				MenuMgr->OnShowPopUp("CAN'T USE THIS ITEM \n ON THIS GAME MODE!", 1);
				return;
			}
			if (IsCanUse(ListItemName[2]))
			{
				ProcessUse(ListItemName[2]);
				APMgr->SetUseTheBoost(TYPE_BOOST::B_LIFE_BOOST, true);
				p_DisplayString.find("tf_use_3")->second->SetStringName("USING..");
			}
		}
	}
}

void BoostMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void BoostMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void BoostMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::BOOSTMENU);
	OnCheckStatusBtn();
	OnCheckVisibleBtn();
	OnFadeIn();
}
void BoostMenu::OnHide()
{
	OnFadeOut();
}

void BoostMenu::OnCheckVisibleBtn()
{
	if (MenuMgr->IsInActionPhase())
	{
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			if ((p_ListButton.at(i)->GetName() == "btn_buy_1") ||
				(p_ListButton.at(i)->GetName() == "btn_buy_2") ||
				(p_ListButton.at(i)->GetName() == "btn_buy_3"))
			{
				p_ListButton.at(i)->Visible(false);
			}
			else if ((p_ListButton.at(i)->GetName() == "btn_use_1") ||
				(p_ListButton.at(i)->GetName() == "btn_use_2") ||
				(p_ListButton.at(i)->GetName() == "btn_use_3"))
			{
				p_ListButton.at(i)->Visible(true);
			}

		}
		p_DisplayString.find("tf_buy_1")->second->IsVisible(false);
		p_DisplayString.find("tf_buy_2")->second->IsVisible(false);
		p_DisplayString.find("tf_buy_3")->second->IsVisible(false);
		//
		p_DisplayString.find("tf_use_1")->second->IsVisible(true);
		p_DisplayString.find("tf_use_2")->second->IsVisible(true);
		p_DisplayString.find("tf_use_3")->second->IsVisible(true);
	}
	else
	{
		for (int i = 0; i < p_ListButton.size(); i++)
		{
			if ((p_ListButton.at(i)->GetName() == "btn_buy_1") ||
				(p_ListButton.at(i)->GetName() == "btn_buy_2") ||
				(p_ListButton.at(i)->GetName() == "btn_buy_3"))
			{
				p_ListButton.at(i)->Visible(true);
			}
			else if ((p_ListButton.at(i)->GetName() == "btn_use_1") ||
				(p_ListButton.at(i)->GetName() == "btn_use_2") ||
				(p_ListButton.at(i)->GetName() == "btn_use_3"))
			{
				p_ListButton.at(i)->Visible(false);
			}
		}
		p_DisplayString.find("tf_use_1")->second->IsVisible(false);
		p_DisplayString.find("tf_use_2")->second->IsVisible(false);
		p_DisplayString.find("tf_use_3")->second->IsVisible(false);
		//
		p_DisplayString.find("tf_buy_1")->second->IsVisible(true);
		p_DisplayString.find("tf_buy_2")->second->IsVisible(true);
		p_DisplayString.find("tf_buy_3")->second->IsVisible(true);
	}
}

void BoostMenu::OnCheckStatusBtn()
{
	auto list = XMLMgr->GetListResourceUI(p_listMenu[p_IdxMenuData]);
	//
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		if (it->second->type == "font")
		{
			if (it->first == "tf_use_1" || it->first == "tf_use_2" || it->first == "tf_use_3")
			{
				p_DisplayString.find(it->first)->second->SetStringName(it->second->source);
			}
		}
	}
	//
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_GRAVITY_BOOST))
	{
		p_DisplayString.find("tf_use_1")->second->SetStringName("USING..");
	}
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_JUMP_BOOST))
	{
		p_DisplayString.find("tf_use_2")->second->SetStringName("USING..");
	}
	if (APMgr->IsUseTheBoost(TYPE_BOOST::B_LIFE_BOOST))
	{
		p_DisplayString.find("tf_use_3")->second->SetStringName("USING..");
	}
}

void BoostMenu::OnParseDetailItem(BoostItem * b)
{
	if (b->p_name == ListItemName[0])
	{
		std::string str = "price: " + std::to_string(b->p_cost);
		p_DisplayString.find("price_1")->second->SetStringName(str);
		str = "remain: " + std::to_string(b->p_remain);
		p_DisplayString.find("remain_1")->second->SetStringName(str);
	}
	else if (b->p_name == ListItemName[1])
	{
		std::string str = "price: " + std::to_string(b->p_cost);
		p_DisplayString.find("price_2")->second->SetStringName(str);
		str = "remain: " + std::to_string(b->p_remain);
		p_DisplayString.find("remain_2")->second->SetStringName(str);
	}
	else if (b->p_name == ListItemName[2])
	{
		std::string str = "price: " + std::to_string(b->p_cost);
		p_DisplayString.find("price_3")->second->SetStringName(str);
		str = "remain: " + std::to_string(b->p_remain);
		p_DisplayString.find("remain_3")->second->SetStringName(str);
	}
}

bool BoostMenu::IsCanBuy(std::string name)
{
	if (MenuMgr->Getcoin() < BoostMgr->GetItem(name)->p_cost)
	{
		MenuMgr->OnShowPopUp("DON'T HAVE ENOUGHT COIN !\nBUY MORE COIN NOW ?", 2, "callIAPMenu", (void*)this, BoostMenu::CallBackFunction);
		return false;
	}
	return true;
}
void BoostMenu::ProcessBuy(std::string name)
{
	//
	int cur_coin = MenuMgr->Getcoin();
	cur_coin = cur_coin - BoostMgr->GetItem(name)->p_cost;
	CCASSERT(cur_coin >= 0, "remain coin must be greater than 0 !");
	MenuMgr->SetCoin(cur_coin);
	//
	int current_item = BoostMgr->GetItemData(name);
	current_item = current_item + BoostMgr->GetItem(name)->p_Number;
	BoostMgr->SetItemData(name, current_item);
	//
	//save the game//
	SaveMgr->SaveTheGameSave();
	//
}
bool BoostMenu::IsCanUse(std::string name, bool IsShowMessage)
{
	if (BoostMgr->GetItem(name)->p_remain <= 0 && MenuMgr->Getcoin() >= BoostMgr->GetItem(name)->p_cost)
	{
		if (IsShowMessage)
		{
			std::string str = "YOU DONT HAVE ANY " + BoostMgr->GetItem(name)->DisplayName + " !\nBUY MORE " + std::to_string(BoostMgr->GetItem(name)->p_Number) + " ITEMS WITH " + std::to_string(BoostMgr->GetItem(name)->p_cost) + " COINS ?";
			MenuMgr->OnShowPopUp(str, 2, std::string("process_buy_" + name), (void*)this, BoostMenu::CallBackFunction);
		}
		return false;
	}
	else if (BoostMgr->GetItem(name)->p_remain <= 0 && MenuMgr->Getcoin() < BoostMgr->GetItem(name)->p_cost)
	{
		if (IsShowMessage)
		{
			std::string str = "YOU DONT HAVE ENOUGH COIN \n TO USE " + BoostMgr->GetItem(name)->DisplayName + ", BUY MORE COIN NOW ?";
			MenuMgr->OnShowPopUp(str, 2, "callIAPMenu", (void*)this, BoostMenu::CallBackFunction);
		}
		return false;
	}
	return true;
}
void BoostMenu::ProcessUse(std::string name, bool IsUse)
{
	int current_item = BoostMgr->GetItemData(name);
	if (IsUse == true)
	{
		current_item = current_item - 1;
	}
	else
	{
		current_item = current_item + 1;
	}
	//
	HUDMenu::GetInstance()->OnCabrivableNumberBoostItem();
	BoostMgr->SetItemData(name, current_item);
	//save the game//
	SaveMgr->SaveTheGameSave();
}
void BoostMenu::CallBackFunction(void * p_Object, std::string str)
{
	BoostMenu * self = (BoostMenu*)p_Object;
	self->OnProcess(str);
}