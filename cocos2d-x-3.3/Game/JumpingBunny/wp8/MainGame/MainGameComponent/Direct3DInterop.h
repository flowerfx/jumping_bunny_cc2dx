/****************************************************************************
Copyright (c) 2013 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once

#include <wrl/client.h>
#include <mutex>
#include <queue>
#include <memory>
#include "Cocos2dRenderer.h"
#include "InputEvent.h"
#include <DrawingSurfaceNative.h>
#include "BasicTimer.h"

namespace cocos2d
{

public delegate void RequestAdditionalFrameHandler();
public delegate void CCOpenConnectFacebookPage(void);
public delegate void DelegateExitApp(void);
public delegate Platform::String ^ DelegateGetString(void);
public delegate int  DelegateGetInt(void);
public delegate void  DelegateGetVoid(void);
public delegate void  DelegateGetVoidFromString(Platform::String ^);
public delegate void  DelegateGetVoidFrom2String(Platform::String ^, Platform::String ^);
public delegate void  DelegateGetVoidFromInt(int);
public delegate bool  DelegateGetGetBool(void);
public delegate void  DelegateGetVoidFromBool(bool);
public delegate Platform::String ^  DelegateGetStringFromString(Platform::String ^);
[Windows::Foundation::Metadata::WebHostHidden]
public ref class Direct3DInterop sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
{
public:
    Direct3DInterop(Windows::Graphics::Display::DisplayOrientations orientation);

	Windows::Phone::Graphics::Interop::IDrawingSurfaceBackgroundContentProvider^ CreateContentProvider();

    // IDrawingSurfaceManipulationHandler
    virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);

	event RequestAdditionalFrameHandler^ RequestAdditionalFrame;


    void UpdateForWindowSizeChange(float width, float height);
    Windows::Foundation::IAsyncAction^ OnSuspending();

    void OnBackKeyPress();
    void OnCocos2dKeyEvent(Cocos2dKeyEvent key);
    void OnCocos2dKeyEvent(Cocos2dKeyEvent key, Platform::String^ text);
    void OnCocos2dEditboxEvent(Platform::Object^ sender, Platform::String^ args, Windows::Foundation::EventHandler<Platform::String^>^ handler);
    void OnOrientationChanged(Windows::Graphics::Display::DisplayOrientations orientation);
    void OnCocos2dOpenURL(Platform::String^ url);

    property Windows::Graphics::Display::DisplayOrientations WindowOrientation;

    property Windows::Foundation::Size WindowBounds;
	property Windows::Foundation::Size NativeResolution;
	property Windows::Foundation::Size RenderResolution;

    void SetCocos2dEventDelegate(Cocos2dEventDelegate^ delegate);
    void SetCocos2dMessageBoxDelegate(Cocos2dMessageBoxDelegate ^ delegate);
    void SetCocos2dEditBoxDelegate(Cocos2dEditBoxDelegate ^ delegate);
    void SetCocos2dOpenURLDelegate(Cocos2dOpenURLDelegate ^ delegate);
	
	//chien todo:
	static event CCOpenConnectFacebookPage^ m_openFacebookPage;
	static void openFacebookPage(){
		m_openFacebookPage();
	}
	static event DelegateExitApp^ m_ExitApp;
	static void Exit_App(){
		m_ExitApp();
	}
	static event DelegateGetGetBool^ m_Get_LogoState;
	static bool Get_LogoState(){
		return m_Get_LogoState();
	}

	static event DelegateGetString^ m_FB_GetToken;
	static Platform::String ^ Get_FB_Token()
	{
		return m_FB_GetToken();
	}
	static event DelegateGetString^ m_FB_GetID;
	static Platform::String ^ Get_FB_ID()
	{
		return m_FB_GetID();
	}
	static event DelegateGetString^ m_FB_GetName;
	static Platform::String ^ Get_FB_Name()
	{
		return m_FB_GetName();
	}
	static event DelegateGetString^ m_FB_GetImgURL;
	static Platform::String ^ Get_FB_ProfileImgURL()
	{
		return m_FB_GetImgURL();
	}
	static event DelegateGetStringFromString^ m_FB_GetImgURLfromID;
	static Platform::String ^ Get_FB_ProfileImgURLFromID(Platform::String ^str)
	{
		return m_FB_GetImgURLfromID(str);
	}
	static event DelegateGetString^ m_FB_GetScore;
	static Platform::String ^ Get_FB_GetScore()
	{
		return m_FB_GetScore();
	}
	static event DelegateGetVoid^ m_FB_RequestScore;
	static void Request_FB_Score()
	{
		m_FB_RequestScore();
	}
	static event DelegateGetVoidFromInt^ m_FB_UpdateScore;
	static void Get_FB_UpdateScore(int s)
	{
		m_FB_UpdateScore(s);
	}
	static event DelegateGetVoidFromString^ m_FB_ShareFB;
	static void Get_FB_ShareFB(Platform::String ^ str)
	{
		m_FB_ShareFB(str);
	}
	static event DelegateGetGetBool^ m_FB_SharePanelAppear;
	static bool Get_FB_SharePanelAppear()
	{
		return m_FB_SharePanelAppear();
	}
	static event DelegateGetVoid^ m_FB_CloseSharePanel;
	static void FB_CloseSharePanel()
	{
		m_FB_CloseSharePanel();
	}

	static event DelegateGetGetBool^ m_IsHaveConnection;
	static bool Get_IsHaveConnection()
	{
		return m_IsHaveConnection();
	}
	static event DelegateGetString^ m_Get_DeviceID;
	static Platform::String^ Get_DeviceID()
	{
		return m_Get_DeviceID();
	}
	static event DelegateGetVoid^ m_RateGame;
	static void OnRateTheGame()
	{
		return m_RateGame();
	}
	/******************************
	*		In App Purchase
	*
	*******************************/
	static event DelegateGetInt^ m_IAP_GetStateLoadItem;
	static int IAP_GetStateLoadItem()
	{
		return m_IAP_GetStateLoadItem();
	}

	static event DelegateGetInt^ m_IAP_GetStatePurchaseItem;
	static int IAP_GetStatePurchaseItem()
	{
		return m_IAP_GetStatePurchaseItem();
	}
	static event DelegateGetVoidFromInt^ m_IAP_SetStateLoadItem;
	static void IAP_SetStateLoadItem(int s)
	{
		m_IAP_SetStateLoadItem(s);
	}
	static event DelegateGetVoidFromInt^ m_IAP_SetStatePurchaseItem;
	static void IAP_SetStatePurchaseItem(int s)
	{
		m_IAP_SetStatePurchaseItem(s);
	}
	static event DelegateGetVoidFromString^ m_IAP_PurchaseItemWithID;
	static void IAP_PurchaseItemWithID(Platform::String^ id)
	{
		m_IAP_PurchaseItemWithID(id);
	}
	static event DelegateGetVoidFrom2String^ m_IAP_InitStoreItem;
	static void IAP_InitStoreItem(Platform::String^ id, Platform::String^ key)
	{
		m_IAP_InitStoreItem(id,key);
	}
	static event DelegateGetVoid^ m_IAP_RequestListItem;
	static void IAP_RequestListItem()
	{
		m_IAP_RequestListItem();
	}
	static event DelegateGetVoidFromString^ m_IAP_DoFulfillmentItemId;
	static void IAP_DoFulfillmentItemId(Platform::String^ id)
	{
		m_IAP_DoFulfillmentItemId(id);
	}

	static event DelegateGetStringFromString^ m_IAP_CheckFulfillmentIfNotFulfill;
	static Platform::String^ IAP_CheckFulfillmentIfNotFulfill(Platform::String^ keygain)
	{
		return m_IAP_CheckFulfillmentIfNotFulfill(keygain);
	}

	static event DelegateGetGetBool^ m_IAP_IsRemoveAds;
	static bool IAP_IsRemoveAds()
	{
		return m_IAP_IsRemoveAds();
	}

	static event DelegateGetString^ m_IAP_GetPriceValue;
	static Platform::String^ IAP_GetPriceValue()
	{
		return m_IAP_GetPriceValue();
	}
	/******************************
	*		In App Purchase
	*
	*******************************/
	static event DelegateGetGetBool^ m_PopUp_GetClose;
	static bool PopUp_GetClose()
	{
		return m_PopUp_GetClose();
	}
	static event DelegateGetVoidFromBool^ m_PopUp_SetClose;
	static void PopUp_SetClose(bool b)
	{
		m_PopUp_SetClose(b);
	}
	/******************************
	*		ADS
	*
	*******************************/
	static event DelegateGetVoid^ m_Ads_ShowBanner;
	static void Ads_ShowBanner()
	{
		m_Ads_ShowBanner();
	}
	static event DelegateGetVoid^ m_Ads_HideBanner;
	static void Ads_HideBanner()
	{
		m_Ads_HideBanner();
	}
	static event DelegateGetGetBool^ m_Ads_IsAppearing;
	static void Ads_IsAppearing()
	{
		m_Ads_IsAppearing();
	}
	/******************************
	*		ADS
	*
	*******************************/
protected:
    // Event Handlers
    void OnPointerPressed(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
    void OnPointerMoved(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
    void OnPointerReleased(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);

internal:
	HRESULT Connect(_In_ IDrawingSurfaceRuntimeHostNative* host, _In_ ID3D11Device1* device);
    void Disconnect();
	HRESULT PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Inout_ DrawingSurfaceSizeF* desiredRenderTargetSize);
    HRESULT Draw(_In_ ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, _In_ ID3D11RenderTargetView* renderTargetView);
    bool SendCocos2dEvent(Cocos2dEvent event);

private:
    Cocos2dRenderer^ m_renderer;
    Windows::Graphics::Display::DisplayOrientations mCurrentOrientation;


    Cocos2dEventDelegate^ m_delegate;
    Cocos2dMessageBoxDelegate^ m_messageBoxDelegate;
    Cocos2dEditBoxDelegate^ m_editBoxDelegate;
    Cocos2dOpenURLDelegate^ m_openURLDelegate;

	int m_FrameCounter;
	float m_ElapsedTimeInSecond;
	BasicTimer^ m_timer;
};

}
