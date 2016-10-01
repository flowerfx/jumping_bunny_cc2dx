#include "IAPManager.h"
#include "FileManager.h"
#include "Init/DisplayView.h"
#include "Init/ParamTexture.h"
#include "Menu/IAPMenu.h"
#include "FXFlatform/XFlatform.h"


extern std::vector<std::string> split(const std::string& s, char seperator);
IAPManager * IAPManager::m_Instance = NULL;


STATE_IAP_LOAD IAPManager::StateGetPurchaseItemProcess()
{
	STATE_IAP_LOAD state = XFlatform::IAP_GetStatePurchaseItem();
	return state;
}
STATE_IAP_LOAD IAPManager::StateGetAllItemProcess()
{
	STATE_IAP_LOAD state = XFlatform::IAP_GetStateLoadItem();
	if (state == STATE_IAP_LOAD::IAP_SUCCEED && !p_IsGetAllPriceValue)
	{ 
		std::wstring w_str = XFlatform::GetPriceValue();
		std::string str = std::string(CCUnicodeToUtf8(w_str.data()));
		auto listStr = split(str, '#');
		IAPMenu::GetInstance()->SetPriceValue("price_pack_1", listStr[1]);
		IAPMenu::GetInstance()->SetPriceValue("price_pack_2", listStr[2]);
		IAPMenu::GetInstance()->SetPriceValue("price_pack_3", listStr[3]);
		IAPMenu::GetInstance()->SetPriceValue("price_pack_ads", listStr[4]);
		p_IsGetAllPriceValue = true;
	}
	return state;
}

void IAPManager::ClearStateLoadItemProcess()
{
	XFlatform::IAP_SetStateLoadItem(STATE_IAP_LOAD::IAP_NONE);
}

void IAPManager::ClearStatePurchaseItemProcess()
{
	XFlatform::IAP_SetStatePurchaseItem(STATE_IAP_LOAD::IAP_NONE);
}

IAPManager::IAPManager()
{
	p_IsOnProcessItem = false;
	p_IsOnLoadingItem = false;
	p_IsRemoveAds = false;
	p_IsGetAllPriceValue = false;
    p_ItemXML = NULL;
}
IAPManager::~IAPManager()
{
    p_ItemXML = NULL;
}
/////////////////////////////////////////
void IAPManager::OnInitialize()
{
	//
	InitValue();
	//
	p_ThreadCheckFulfillment = std::thread(&IAPManager::CheckItemFulfillmentAtInit,this);
	p_IsCheckFulfillmentComplete = false;
}

bool IAPManager::OnGetInventoryStore()
{
	if (!p_IsCheckFulfillmentComplete)
	{
		return false;
	}
	XFlatform::IAP_RequestListItem();
	return true;
}

void IAPManager::OnPurchaseItem(int idx)
{
	std::string id = p_listIAPItem.at(idx)->p_IdIAP;
	XFlatform::IAP_PurchaseItemWithID(id);
	//
	MenuMgr->OnShowPopUp("PURCHASING ITEM ...", 0, "");
}

void IAPManager::DoFulfillmentItem(std::string key)
{
	XFlatform::IAP_DoFulfillmentItemId(key);
}

void IAPManager::OnpurchaseResult(STATE_IAP_LOAD result, int idx)
{
	if (result == STATE_IAP_LOAD::IAP_SUCCEED && idx >= 0)
	{
		//
		if (p_listIAPItem.at(idx)->p_Gain == 1) // is bought remove ads
		{
			p_IsRemoveAds = true;
			XFlatform::Ads_HideBanner();
			SaveMgr->SetRemoveAds("1");
		}
		else
		{
			int cur_coin = MenuMgr->Getcoin();
			cur_coin = cur_coin + p_listIAPItem.at(idx)->p_Gain;
			MenuMgr->SetCoin(cur_coin, true);
		}
		SaveMgr->SaveTheGameSave();
		//
		MenuMgr->OnShowPopUp("TRANSACTION SUCCEED!", 1, "");
		//
		DoFulfillmentItem(p_listIAPItem.at(idx)->p_IdIAP);
		//
		//clear state purchase iap
		ClearStatePurchaseItemProcess();
	}
	else if (result == STATE_IAP_LOAD::IAP_FAIL)
	{
		MenuMgr->OnShowPopUp("TRANSACTION FAIL!\nTRY AGAIN LATER!", 1, "");
		//
		//clear state purchase iap
		ClearStatePurchaseItemProcess();
	}
}
///////////////////////////////////////////
void IAPManager::InitValue()
{
	LoadXMLData();
	//
	OnParseValueToIAPMenu();
}
void IAPManager::LoadXMLData()
{
	if (!p_ItemXML)
	{
		p_ItemXML = new TiXmlDocument();
	}
#if USE_ARK_ARCHIVE
	p_ItemXML->LoadFileArk(std::string(XML_DIR) + std::string(ITEM_XML));
	OnParseData();
#else
	FILE * f = GetFile((std::string(XML_DIR) + std::string(ITEM_XML)).c_str(), "rb");
	if (f)
	{
		p_ItemXML->LoadFile(f);
		OnParseData();
		fclose(f);
	}
#endif
}


IAPItem* IAPManager::GetItem(std::string nameItem)
{
	for (size_t i = 0; i < p_listIAPItem.size(); i++)
	{
		if (p_listIAPItem.at(i)->p_name == nameItem)
		{
			return p_listIAPItem.at(i);
		}
	}
	return NULL;
}

void IAPManager::OnParseData()
{
	auto root = p_ItemXML->FirstChildElement("GAME");
	if (!root)
	{
		CCASSERT(false, "root data not found !");
		return;
	}
	auto ListIAPItem = root->FirstChildElement("IAPItem");
	if (ListIAPItem)
	{
		auto detail = ListIAPItem->FirstChildElement("Item");
		do
		{
			if (detail)
			{
				auto item = new IAPItem();
				item->p_name = detail->Attribute("name");
				item->p_IdIAP = detail->Attribute("key");
				item->p_cost = detail->Attribute("cost");
				item->p_Gain = atoi(detail->Attribute("gain"));
				item->DisplayName = detail->Attribute("displayName");
				p_listName.push_back(item->p_name);
				p_listIAPItem.push_back(item);

				detail = detail->NextSiblingElement();
			}

		} while (detail);
	}
}

void IAPManager::OnParseValueToIAPMenu()
{
	for (size_t i = 0; i < p_listIAPItem.size(); i++)
	{
		IAPMenu::GetInstance()->OnParseDetailIAPItem(p_listIAPItem.at(i));
		//
		std::string id = p_listIAPItem.at(i)->p_IdIAP;
		std::string key = p_listIAPItem.at(i)->p_name;
		XFlatform::IAP_InitStoreItem(id, key);
	}

}

void IAPManager::CheckItemFulfillmentAtInit()
{
	std::string	keyGain = "";
	//check internet here
	//may be low on this so we must check in another thread
	if (XFlatform::IsHaveInternetConnection())
	{	
		keyGain = XFlatform::IAP_CheckFulfillmentIfNotFulfill("");
	}
	if (keyGain != "")
	{	
		auto listStr = split(keyGain, '#');
		int cur_coin = MenuMgr->Getcoin();
		for (int i = 0; i < listStr.size(); i++)
		{
			if (listStr.at(i) == "_bunny_small_pack")
			{
				cur_coin = cur_coin + p_listIAPItem.at(0)->p_Gain;
				MenuMgr->SetCoin(cur_coin, true);
			}
			else if (listStr.at(i) == "_bunny_medium_pack")
			{
				cur_coin = cur_coin + p_listIAPItem.at(0)->p_Gain;
				MenuMgr->SetCoin(cur_coin, true);
			}
			else if (listStr.at(i) == "_bunny_mega_pack")
			{
				cur_coin = cur_coin + p_listIAPItem.at(0)->p_Gain;
				MenuMgr->SetCoin(cur_coin, true);
			}
		}
		SaveMgr->SaveTheGameSave();
	}
	p_IsRemoveAds = XFlatform::IAP_IsAdsRemove();
	if (p_IsRemoveAds == false)
	{
		p_IsRemoveAds = (SaveMgr->isRemoveAds() == "1");
	}

	ShowAds();
	//
	p_IsCheckFulfillmentComplete = true;
}

void IAPManager::ShowAds()
{
	if (!p_IsRemoveAds)
	{
		XFlatform::Ads_ShowBanner();
	}
}