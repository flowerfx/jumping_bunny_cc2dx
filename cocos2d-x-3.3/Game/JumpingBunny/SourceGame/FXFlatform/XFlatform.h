#ifndef _XFLATFORM_H_
#define _XFLATFORM_H_
#include "Utility/IAPManager.h"
namespace XFlatform
{
	bool IsAtLogoState();
	bool IsHaveInternetConnection();
	void OnRateTheGame();

	void FB_OpenLoginPage();
	void FB_SharePageString(std::wstring str);
	std::string FB_GetUserScore();
	void FB_RequestUserScore();
	void FB_UpdateUserScore(unsigned int i);
	std::wstring FB_GetUserProfile(std::string key);
	std::wstring FB_GetProfileImgURLFromID(std::wstring id);
	bool FB_IsSharePanelAppear();
	void FB_CloseSharePanel();

	void PopUp_SetClose(bool b);
	bool PopUp_GetClose();

	STATE_IAP_LOAD IAP_GetStatePurchaseItem();
	STATE_IAP_LOAD IAP_GetStateLoadItem();
	void IAP_SetStateLoadItem(STATE_IAP_LOAD s);
	void IAP_SetStatePurchaseItem(STATE_IAP_LOAD s);
	void IAP_RequestListItem();
	void IAP_PurchaseItemWithID(std::string id);
	void IAP_DoFulfillmentItemId(std::string id);
	void IAP_InitStoreItem(std::string id, std::string key);
	std::string IAP_CheckFulfillmentIfNotFulfill(std::string keygain);
	bool IAP_IsAdsRemove();
	std::wstring GetPriceValue();

	void Ads_ShowBanner();
	void Ads_HideBanner();
}


#endif //_XFLATFORM_H_