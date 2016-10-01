//
//  IOSXFlatform.h
//  JumpingBunny_IOS
//
//  Created by qchien on 5/2/15.
//
//

#ifndef JumpingBunny_IOS_IOSXFlatform_h
#define JumpingBunny_IOS_IOSXFlatform_h
#ifdef OS_IOS
#include "wchar.h"
#include "Utility/IAPManager.h"

namespace XFlatform
{
    bool IOS_IsAtLogoState();
    bool IOS_IsHaveInternetConnection();
    void IOS_OnRateTheGame();
    
    void IOS_FB_OpenLoginPage();
    void IOS_FB_SharePageString(std::wstring str);
    const char*  IOS_FB_GetUserScore();
    void IOS_FB_RequestUserScore();
    void IOS_FB_UpdateUserScore(unsigned int i);
    const wchar_t* IOS_FB_GetUserProfile(std::string key);
    const wchar_t* IOS_FB_GetProfileImgURLFromID(std::wstring id);
    bool IOS_FB_IsSharePanelAppear();
    void IOS_FB_CloseSharePanel();
    
    void IOS_PopUp_SetClose(bool b);
    bool IOS_PopUp_GetClose();
    
    STATE_IAP_LOAD IOS_IAP_GetStatePurchaseItem();
    STATE_IAP_LOAD IOS_IAP_GetStateLoadItem();
    void IOS_IAP_SetStateLoadItem(STATE_IAP_LOAD s);
    void IOS_IAP_SetStatePurchaseItem(STATE_IAP_LOAD s);
    void IOS_IAP_RequestListItem();
    void IOS_IAP_PurchaseItemWithID(std::string id);
    void IOS_IAP_DoFulfillmentItemId(std::string id);
    void IOS_IAP_InitStoreItem(std::string id, std::string key);
    const char* IOS_IAP_CheckFulfillmentIfNotFulfill(std::string keygain);
    bool IOS_IAP_IsAdsRemove();
    const wchar_t* IOS_GetPriceValue();
    
    void IOS_Ads_ShowBanner();
    void IOS_Ads_HideBanner();
}
#endif
#endif
