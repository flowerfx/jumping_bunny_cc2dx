#ifndef _IAP_MANAGER_H_
#define _IAP_MANAGER_H_
#pragma once
#include "LoadDataXML.h"
enum STATE_IAP_LOAD
{
	IAP_FAIL = -1,
	IAP_NONE,
	IAP_PROCESSING,
	IAP_SUCCEED
};
struct IAPItem
{
	std::string p_name;
	std::string p_cost;
	std::string p_IdIAP;
	int p_Gain;
	std::string DisplayName;
public:
	IAPItem() {
		p_name = "";
		p_IdIAP= "";
		p_cost = "";
		p_Gain = 0;
		DisplayName = "";
	}
};

static std::string ListIAPItemName[] = { "small_pack", "med_pack", "mega_pack", "remove_ads" };

class IAPManager
{
private:
	static IAPManager * m_Instance;
	std::vector<IAPItem*>		p_listIAPItem;
	std::vector<std::string>	p_listName;
	TiXmlDocument*				p_ItemXML;
	bool p_IsOnProcessItem;
	bool p_IsOnLoadingItem;
	bool p_IsRemoveAds;

	bool p_IsGetAllPriceValue;

	std::thread p_ThreadCheckFulfillment;
	bool p_IsCheckFulfillmentComplete;
public:
	static IAPManager * GetInstance()
	{
		if (!m_Instance)
		{
			m_Instance = new IAPManager();
		}
		return m_Instance;
	}

	STATE_IAP_LOAD StateGetAllItemProcess();
	STATE_IAP_LOAD StateGetPurchaseItemProcess();
	void ClearStateLoadItemProcess();
	void ClearStatePurchaseItemProcess();

	IAPManager();
	~IAPManager();

	void OnInitialize();
	bool OnGetInventoryStore();
	void OnPurchaseItem(int idx);
	void DoFulfillmentItem(std::string key);

	void CheckItemFulfillmentAtInit();
	//result
	void OnpurchaseResult(STATE_IAP_LOAD result, int idx);

	void InitValue();
	void LoadXMLData();

	IAPItem* GetItem(std::string nameItem);
	//
	void ShowAds();
	//
	bool IsRemoveAds() { return p_IsRemoveAds; }
	void SetRemoveAds(bool val) { p_IsRemoveAds = val; }
protected:
	void OnParseData();
	void OnParseValueToIAPMenu();

};

#define IAPMgr IAPManager::GetInstance()
#endif //_IAP_MANAGER_H_