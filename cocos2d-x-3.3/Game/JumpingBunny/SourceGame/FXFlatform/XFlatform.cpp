#include "Init/DisplayView.h"
#ifdef WP8
#include "Direct3DInterop.h"
#endif
#include "XFlatform.h"
#ifdef WP8
extern Platform::String ^stops(std::string s);
extern std::string pstos(Platform::String^ ps);
#endif
namespace XFlatform
{
	bool IsAtLogoState()
	{
#ifdef WP8
		return cocos2d::Direct3DInterop::Get_LogoState();
#else
		return false;
#endif
	}

	bool IsHaveInternetConnection()
	{
#ifdef WP8
		return cocos2d::Direct3DInterop::Get_IsHaveConnection();
#else
		return true;
#endif
	}

	void OnRateTheGame()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::OnRateTheGame();
#else
		return ;
#endif
	}


	void FB_OpenLoginPage()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::openFacebookPage();
#else
		return;
#endif
	}

	void FB_SharePageString(std::wstring str)
	{
#ifdef WP8
		cocos2d::Direct3DInterop::Get_FB_ShareFB(ref new Platform::String(str.data()));
#else
#endif
	}

	std::string FB_GetUserScore()
	{
#ifdef WP8
		std::string score = std::string(CCUnicodeToUtf8(cocos2d::Direct3DInterop::Get_FB_GetScore()->Data()));
		return score;
#else
		return "0";
#endif
	}

	void FB_RequestUserScore()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::Request_FB_Score();
#else
		return;
#endif
	}

	void FB_UpdateUserScore(unsigned int i)
	{
#ifdef WP8
		cocos2d::Direct3DInterop::Get_FB_UpdateScore(i);
#else
		return;
#endif
	}

	std::wstring FB_GetUserProfile(std::string key)
	{
		std::wstring val = L"";
		if (key == "name")
		{
#ifdef WP8
			val = cocos2d::Direct3DInterop::Get_FB_Name()->Data();
#else
			val = L"";
#endif
		}
		else if (key == "id")
		{
#ifdef WP8
			val = cocos2d::Direct3DInterop::Get_FB_ID()->Data();
#else
			val = L"";
#endif
		}
		else if (key == "token")
		{
#ifdef WP8
			val = cocos2d::Direct3DInterop::Get_FB_Token()->Data();
#else
			val = L"";
#endif
		}
		else if (key == "imgUrl")
		{
#ifdef WP8
			val = cocos2d::Direct3DInterop::Get_FB_ProfileImgURL()->Data();
#else
			val = L"";
#endif
		}
		return val;
	}

	std::wstring FB_GetProfileImgURLFromID(std::wstring id)
	{
		std::wstring val = L"";
#ifdef WP8
		val = cocos2d::Direct3DInterop::Get_FB_ProfileImgURLFromID(ref new Platform::String(id.data()))->Data();
#else
		val = L"";
#endif
		return val;
	}

	bool FB_IsSharePanelAppear()
	{
#ifdef WP8
		return cocos2d::Direct3DInterop::Get_FB_SharePanelAppear();
#else
		return false;
#endif
	}

	void FB_CloseSharePanel()
	{
#ifdef WP8
		return cocos2d::Direct3DInterop::FB_CloseSharePanel();
#else
		return ;
#endif
	}

	void PopUp_SetClose(bool b)
	{
#ifdef WP8
		cocos2d::Direct3DInterop::PopUp_SetClose(b);
#else
		return;
#endif
	}

	bool PopUp_GetClose()
	{
#ifdef WP8
		return cocos2d::Direct3DInterop::PopUp_GetClose();
#else
		return true;
#endif
	}
	/*
#ifdef WP8
#else
#endif
	*/
	STATE_IAP_LOAD IAP_GetStatePurchaseItem()
	{
#ifdef WP8
		int res = cocos2d::Direct3DInterop::IAP_GetStatePurchaseItem();
		return (STATE_IAP_LOAD)res;
#else
		return STATE_IAP_LOAD::IAP_SUCCEED;
#endif
	}

	STATE_IAP_LOAD IAP_GetStateLoadItem()
	{
#ifdef WP8
		int res = cocos2d::Direct3DInterop::IAP_GetStateLoadItem();
		return (STATE_IAP_LOAD)res;
#else
		return STATE_IAP_LOAD::IAP_SUCCEED;
#endif
	}

	void IAP_SetStateLoadItem(STATE_IAP_LOAD s)
	{
#ifdef WP8
		cocos2d::Direct3DInterop::IAP_SetStateLoadItem((int)s);
#else
		return;
#endif
	}

	void IAP_SetStatePurchaseItem(STATE_IAP_LOAD s)
	{
#ifdef WP8
		cocos2d::Direct3DInterop::IAP_SetStatePurchaseItem((int)s);
#else
		return;
#endif
	}

	void IAP_RequestListItem()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::IAP_RequestListItem();
#else
		return; 
#endif
	}

	void IAP_PurchaseItemWithID(std::string id)
	{
#ifdef WP8
		Platform::String ^ itemId = stops(id);
		cocos2d::Direct3DInterop::IAP_PurchaseItemWithID(itemId);
#else
		return;
#endif
	}

	void IAP_DoFulfillmentItemId(std::string id)
	{
#ifdef WP8
		Platform::String ^ keyId = stops(id);
		cocos2d::Direct3DInterop::IAP_DoFulfillmentItemId(keyId);
#else
		return;
#endif
	}

	void IAP_InitStoreItem(std::string id, std::string key)
	{
#ifdef WP8
		Platform::String ^ pid = stops(id);
		Platform::String ^ pkey = stops(key);
		cocos2d::Direct3DInterop::IAP_InitStoreItem(pid, pkey);
#else
		return;
#endif
	}

	std::string IAP_CheckFulfillmentIfNotFulfill(std::string keygain)
	{
		std::string returnkey = "";
#ifdef WP8
		Platform::String^	keyGain = "";
		Platform::String^	keyGain1 = "";
		keyGain1 = cocos2d::Direct3DInterop::IAP_CheckFulfillmentIfNotFulfill(keyGain);
		returnkey = pstos(keyGain1);
#else
		returnkey = "";
#endif
		return returnkey;
	}
	bool IAP_IsAdsRemove()
	{
#ifdef WP8
		bool b = cocos2d::Direct3DInterop::IAP_IsRemoveAds();
		return b;
#else
		return true;
#endif
	}

	std::wstring GetPriceValue()
	{
#ifdef WP8
		std::wstring w_str = cocos2d::Direct3DInterop::IAP_GetPriceValue()->Data();
		return w_str;
#else
		return L"";
#endif
	}

	void Ads_ShowBanner()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::Ads_ShowBanner();
#else
		return;
#endif
	}

	void Ads_HideBanner()
	{
#ifdef WP8
		cocos2d::Direct3DInterop::Ads_HideBanner();
#else
		return;
#endif
	}
}

