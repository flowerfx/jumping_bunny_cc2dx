#include "MainMenu.h"
#include "IAPMenu.h"
#include "GamePlay/ManageObjectAP.h"
#if USE_CC_AUDIO
#include "Utility/SoundManager.h"
#endif
using namespace cocos2d;

IAPMenu * IAPMenu::m_Instance = NULL;

IAPMenu::IAPMenu()
{
	p_MenuToSwitchTo = MENU_NONE;
	p_idxItemProcess = -1;

	p_IdxMenuData = 6;
}


IAPMenu::~IAPMenu()
{
}

void IAPMenu::Init()
{
	InitEntity(p_listMenu[p_IdxMenuData]);
}


void IAPMenu::Update(float dt)
{
	onUpdate(dt);
	if (p_DeltaMoveTime <= 0)
	{
		p_DeltaMoveTime = 0;
		if (p_curStatus == STATUS_MENU::M_FADEOUT)
		{
			MenuMgr->SwitchToMenu(p_MenuToSwitchTo);
			//clear state IAP
			IAPMgr->ClearStateLoadItemProcess();
			IAPMgr->ClearStatePurchaseItemProcess();
			//
			//reset object posion to make sure the menu not stuck
			ResetPositionResource();
		}
		//when state is loading or fail do not update the iap menu
		if (IAPMgr->StateGetAllItemProcess() == IAP_FAIL)
		{
			MenuMgr->OnShowPopUp("CONTACT STORE FAIL ! \n TRY AGAIN LATER", 1, "btn_ok", (void*)this, IAPMenu::CallBackFunction);

		}
		if (IAPMgr->StateGetAllItemProcess() != IAP_SUCCEED && IAPMgr->StateGetAllItemProcess() != IAP_NONE)
		{
			return;
		}
		//when state purchase is not idle
		if (IAPMgr->StateGetPurchaseItemProcess() != IAP_NONE)
		{
			STATE_IAP_LOAD s = IAPMgr->StateGetPurchaseItemProcess();
			IAPMgr->OnpurchaseResult(s, p_idxItemProcess);
			if (s != STATE_IAP_LOAD::IAP_PROCESSING)
			{
				p_idxItemProcess = -1;
				//
				std::string str_gold = "$  " + std::to_string(MenuMgr->Getcoin());
				p_DisplayString.find("title_gold")->second->SetStringName(str_gold);
			}
			return;
		}		
		//
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
void IAPMenu::Visit(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch )
{
	onVisit(renderer, transform, flags, spriteBatch);
}
void IAPMenu::Draw(Renderer *renderer, const Mat4& transform, uint32_t flags, SpriteBatchNode * spriteBatch)
{
	onDraw(renderer, transform, flags, spriteBatch);
}

void IAPMenu::OnProcessButton(std::string name)
{
	if (name == "btn_ok")
	{
		OnHide();
		if (!MenuMgr->IsInActionPhase())
		{
			p_MenuToSwitchTo = MENULAYER::MAINMENU;
			MainMenu::GetInstance()->SetMenuReturnTo(MENULAYER::IAP);
		}
		else
		{
			p_MenuToSwitchTo = MENULAYER::ACTION_PHASE;
		}
	}
	else if (name == "btn_buy_pack1")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		IAPManager::GetInstance()->OnPurchaseItem(0);
		//
		p_idxItemProcess = 0;
	}
	else if (name == "btn_buy_pack2")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		IAPManager::GetInstance()->OnPurchaseItem(1);
		//
		p_idxItemProcess = 1;
	}
	else if (name == "btn_buy_pack3")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		IAPManager::GetInstance()->OnPurchaseItem(2);		
		//
		p_idxItemProcess = 2;
	}
	else if (name == "btn_buy_ads")
	{
#if USE_CC_AUDIO
		GetSound->PlaySoundEffect(XMLMgr->GetPathSound("SFX_BTN_BUY"));
#endif
		if (IAPMgr->IsRemoveAds())
		{
			MenuMgr->OnShowPopUp("YOU HAVE BOUGHT THIS ITEM !", 1);
			return;
		}
		IAPManager::GetInstance()->OnPurchaseItem(3);
		//
		p_idxItemProcess = 3;
	}
}

void IAPMenu::OnFadeIn()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEIN;
}
void IAPMenu::OnFadeOut()
{
	p_DeltaMoveTime = XMLMgr->GetUIData()->dataMenu.find(p_listMenu[p_IdxMenuData])->second->deltaTime;
	p_curStatus = STATUS_MENU::M_FADEOUT;
}
void IAPMenu::OnShow()
{
	MenuMgr->SetCurrentMenuLayer(MENULAYER::IAP);
	if (p_MenuReturnTo == IGM)
	{
		p_MenuReturnTo = MENU_NONE;
		return;
	}
	//
	std::string str_gold = "$  " + std::to_string(MenuMgr->Getcoin());
	p_DisplayString.find("title_gold")->second->SetStringName(str_gold);
	//
	OnFadeIn();
	//
	//MenuMgr->OnShowPopUp("PROCESSING...!", 0, "");
	//
	bool res = IAPMgr->OnGetInventoryStore();
	if (!res)
	{
		MenuMgr->OnShowPopUp("ON FULFILLMENT ! \n TRY AGAIN LATER", 1, "btn_ok", (void*)this, IAPMenu::CallBackFunction);
	}
	else
	{
		MenuMgr->OnShowPopUp("PROCESSING...!", 0, "");
	}
}
void IAPMenu::OnHide()
{
	OnFadeOut();
}

void IAPMenu::OnParseDetailIAPItem(IAPItem * b)
{
	if (b->p_name == ListIAPItemName[0])
	{
		std::string str = b->p_cost;
		p_DisplayString.find("price_pack_1")->second->SetStringName(str);
		str = b->DisplayName;
		p_DisplayString.find("title_btn_1")->second->SetStringName(str);
		str = "+ " + std::to_string(b->p_Gain);
		p_DisplayString.find("dec_panel_1")->second->SetStringName(str);
	}
	else if (b->p_name == ListIAPItemName[1])
	{
		std::string str = (b->p_cost);
		p_DisplayString.find("price_pack_2")->second->SetStringName(str);
		str = b->DisplayName;
		p_DisplayString.find("title_btn_2")->second->SetStringName(str);
		str = "+ " + std::to_string(b->p_Gain);
		p_DisplayString.find("dec_panel_2")->second->SetStringName(str);
	}
	else if (b->p_name == ListIAPItemName[2])
	{
		std::string str = (b->p_cost);
		p_DisplayString.find("price_pack_3")->second->SetStringName(str);
		str = b->DisplayName;
		p_DisplayString.find("title_btn_3")->second->SetStringName(str);
		str = "+ " + std::to_string(b->p_Gain);
		p_DisplayString.find("dec_panel_3")->second->SetStringName(str);
	}
	else if (b->p_name == ListIAPItemName[3])
	{
		std::string str = (b->p_cost);
		p_DisplayString.find("price_pack_ads")->second->SetStringName(str);
	}
}
void IAPMenu::CallBackFunction(void * p_Object, std::string str)
{
	IAPMenu * self = (IAPMenu*)p_Object;
	self->OnProcessButton(str);
}

void IAPMenu::SetPriceValue(std::string name, std::string value)
{
	p_DisplayString.find(name)->second->SetStringName(value);
}